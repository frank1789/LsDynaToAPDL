#ifndef MANAGEFILE_H
#define MANAGEFILE_H

#include <QObject>
#include <QString>
#include "reader.h"

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

private:
    void getfileName();

    void setNewfileName();

    QString _fileName;

    unsigned long _counter;

    unsigned long _numofFile;

    QString _NewfileName;

    QString _replace;
};

#endif // MANAGEFILE_H
