#ifndef VNODEDETAIL_H
#define VNODEDETAIL_H

#include <QtGlobal>
#include <QMetaType>
#include "../options.h"

class VNodeDetail
{
public:
    VNodeDetail();
    VNodeDetail(qint64 id, Scene::Type typeTool, Draw::Mode mode);
    VNodeDetail(const VNodeDetail &node);
    qint64 getId() const;
    void setId(const qint64 &value);
    Scene::Type getTypeTool() const;
    void setTypeTool(const Scene::Type &value);
    Draw::Mode getMode() const;
    void setMode(const Draw::Mode &value);
private:
    qint64 id;
    Scene::Type typeTool;
    Draw::Mode mode;
};

Q_DECLARE_METATYPE(VNodeDetail)

#endif // VNODEDETAIL_H
