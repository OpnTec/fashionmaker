#ifndef VNODEDETAIL_H
#define VNODEDETAIL_H

#include <QtGlobal>
#include <QMetaType>
#include "options.h"

namespace NodeDetail{
    enum Type
    {
        Contour,
        Modeling
    };
}

class VNodeDetail
{
public:
    VNodeDetail();
    VNodeDetail(qint64 id, Tools::Enum typeTool, Draw::Mode mode, NodeDetail::Type typeNode);
    VNodeDetail(const VNodeDetail &node);
    qint64 getId() const;
    void setId(const qint64 &value);
    Tools::Enum getTypeTool() const;
    void setTypeTool(const Tools::Enum &value);
    Draw::Mode getMode() const;
    void setMode(const Draw::Mode &value);
    NodeDetail::Type getTypeNode() const;
    void setTypeNode(const NodeDetail::Type &value);

private:
    qint64 id;
    Tools::Enum typeTool;
    Draw::Mode mode;
    NodeDetail::Type typeNode;
};

Q_DECLARE_METATYPE(VNodeDetail)

#endif // VNODEDETAIL_H
