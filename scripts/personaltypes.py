from dumper import *

"""
Qt Creator Debugging Helpers
"""

def qdump__VSAPoint(d, value):
    d.putValue('(%s, %s)' % (value.split('dd')))
    d.putPlainChildren(value)

def qdump__PieceNodeAngle(d, value):
    d.putValue(value.cast(value.type.name).displayEnum())
    d.putNumChild(0)

def qdump__PassmarkAngleType(d, value):
    d.putValue(value.cast(value.type.name).displayEnum())
    d.putNumChild(0)

def qdump__PassmarkLineType(d, value):
    d.putValue(value.cast(value.type.name).displayEnum())
    d.putNumChild(0)

def qdump__Tool(d, value):
    d.putValue(value.cast(value.type.name).displayEnum())
    d.putNumChild(0)
