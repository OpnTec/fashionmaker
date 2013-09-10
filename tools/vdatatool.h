#ifndef VDATATOOL_H
#define VDATATOOL_H

#include <QObject>
#include "container/vcontainer.h"

class VDataTool : public QObject
{
    Q_OBJECT
public:
    explicit VDataTool(VContainer *data, QObject *parent = 0);
    virtual ~VDataTool();
    const VDataTool& operator= (const VDataTool &tool);
    VContainer getData() const;
    void setData(const VContainer *value);

signals:
    
public slots:

protected:
    VContainer   data;
    
};

#endif // VDATATOOL_H
