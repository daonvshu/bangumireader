#pragma once

#include <qobject.h>
#include <functional>
#include <qexception.h>
#include <qdebug.h>

class TextParseException : public QException {
public:
    void raise() const override { throw* this; }
    TextParseException* clone() const override { return new TextParseException(*this); }
};

namespace TextUtils {

    struct El {
        enum Type {
            None,
            AND,
            OR,
            Keywords,
            SubEl,
        };
        Type type = None;
        QString keywords;
        El* parent = nullptr;

        QList<El> els;
        int keywordsIndexBegin;

        bool isOperator() const {
            return type == AND || type == OR;
        }

        bool lastIsOperator() const {
            return els.last().isOperator();
        }

        bool isKeywords() const {
            return type == Keywords;
        }

        bool lastIsKeywords() const {
            return els.last().isKeywords();
        }

        bool isSubEl() const {
            return type == SubEl;
        }

        bool lastIsSubEl() const {
            return els.last().isSubEl();
        }

        void keywordsEndWith(const QString& source, int endIndex) {
            keywords = source.mid(keywordsIndexBegin, endIndex - keywordsIndexBegin);
        }

        void printTree(int depth) const {
            for (const auto& el: els) {
                qDebug() << QString("-").repeated(depth) << [&, depth]() -> QString {
                    switch (el.type) {
                        case None: return "none";
                        case AND: return "AND";
                        case OR: return "OR";
                        case Keywords: return "key:" + el.keywords;
                        case SubEl: break;
                    }
                    el.printTree(depth + 1);
                    return "";
                }();
            }
        }

        bool testStr(const std::function<bool(const QString&)>& tester) const {
            bool value = true;
            QList<Type> operators;

            for (const auto& el: els) {
                if (el.type == Keywords) {
                    if (operators.isEmpty()) {
                        value = tester(el.keywords);
                    } else {
                        auto op = operators.takeLast();
                        if (op == AND) {
                            value = value && tester(el.keywords);
                        } else if (op == OR) {
                            value = value || tester(el.keywords);
                        }
                    }
                } else if (el.type == SubEl) {
                    if (operators.isEmpty()) {
                        value = el.testStr(tester);
                    } else {
                        auto op = operators.takeLast();
                        if (op == AND) {
                            value = value && el.testStr(tester);
                        } else if (op == OR) {
                            value = value || el.testStr(tester);
                        }
                    }
                } else if (el.isOperator()) {
                    operators << el.type;
                }
            }
            return value;
        }
    };

    template<typename T>
    static QList<T> filterByKeywords(const QString& keywords, const QList<T>& data, const std::function<QString(const T&)>& mapper) {
        //(A&B)|C
        //A&(B|C)
        //A|B&(C)
        //((A|B)&C)
        //A&B|C
        if (keywords.isEmpty()) {
            throw TextParseException();
        }

        El el;
        El* currentLoadEl = &el;

        //load el tree
        for (int i = 0; i < keywords.size(); i++) {
            const auto& c = keywords.at(i);
            if (c == '(') {
                if (!currentLoadEl->els.isEmpty()) {
                    if (!currentLoadEl->lastIsOperator()) {
                        throw TextParseException();
                    }
                }
                currentLoadEl->els << El{ El::SubEl, "", currentLoadEl };
                currentLoadEl = &currentLoadEl->els.last();
            } else if (c == '&' || c == '|') {
                if (currentLoadEl->els.isEmpty() || (!currentLoadEl->lastIsKeywords() && !currentLoadEl->lastIsSubEl())) {
                    throw TextParseException();
                }
                currentLoadEl->els.last().keywordsEndWith(keywords, i);
                currentLoadEl->els << El{ c == '&' ? El::AND : El::OR, "", currentLoadEl->parent };
            } else if (c == ')') {
                if (currentLoadEl->els.isEmpty() || currentLoadEl->lastIsOperator()) {
                    throw TextParseException();
                }
                if (currentLoadEl->lastIsKeywords()) {
                    currentLoadEl->els.last().keywordsEndWith(keywords, i);
                }
                currentLoadEl = currentLoadEl->parent;
            } else {
                if (!currentLoadEl->els.isEmpty() && currentLoadEl->lastIsKeywords()) {
                    continue;
                }
                currentLoadEl->els << El{ El::Keywords, "", currentLoadEl->parent, {}, i };
            }
        }

        if (currentLoadEl->els.isEmpty()) {
            throw TextParseException();
        }
        if (currentLoadEl->lastIsKeywords()) {
            currentLoadEl->els.last().keywordsEndWith(keywords, keywords.size());
        }
        if (currentLoadEl->lastIsOperator()) {
            throw TextParseException();
        }
        el.printTree(1);

        //filter keywords
        QList<T> filterResults;
        for (const auto& d: data) {
            auto dataStr = mapper(d);
            if (el.testStr([&](const QString& k) {
                return dataStr.contains(k);
            })) {
                filterResults << d;
            }
        }

        return filterResults;
    }
}
