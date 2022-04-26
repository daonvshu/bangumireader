#include "remarktbmodel.h"

#include "dao.h"
#include "entity/RemarkTb.h"

bool RemarkTbModel::checkLinkExist(const QString& link) {
    RemarkTb::Fields rf;
    return dao::_count<RemarkTb>().filter(rf.sourceLink == link).count() != 0;
}

void RemarkTbModel::cacheNewDownloadedLink(const QString& link) {
    if (checkLinkExist(link)) {
        return;
    }
    RemarkTb tb(link);
    dao::_insert<RemarkTb>().build().insert(tb);
}
