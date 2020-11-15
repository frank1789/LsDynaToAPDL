

TEMPLATE = lib
TARGET = common

CONFIG += \
    staticlib \
    c++17

QT += core 

# Input
HEADERS += \
    finite_element_types.h \
    logger_tools.h \
    node_type_impl.h 
