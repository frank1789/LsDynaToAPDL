#ifndef MANAGEFILE_H
#define MANAGEFILE_H

#include <QObject>
#include <QString>
#include "reader.h"


/**
 * @brief The ManageFile class manages incoming files.
 *
 * @details It allows to extract the information of the incoming files as:
 * name, extension, size, etc.
 * Defines the name of the output file and checks if it already exists
 */
class ManageFile : public QObject
{
    Q_OBJECT

public:
    ManageFile();

    ~ManageFile();

    void print();

    int getSizelistFile();

    void convert(ConverterSintax *pConverter, Node* node, Shell* shell);

public slots:
    void setSizelist(int size);

    void setFile(const QString &fileName);

signals:
    void setconvertedname(QString &nameFile);

    void setfileName(const QString &fileName);

    void outputfileName(const QString &fileName);

    void getPropertyFile(const qint64 &dimension, const QString &label);

private:
    void getfileName();

    void setNewfileName();

    void setPropertyFile();

    QString _fileName;

    QString _NewfileName;

    QString _replace;

    unsigned long _counter;

    unsigned long _numofFile;
};

#endif // MANAGEFILE_H
