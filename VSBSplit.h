#ifndef CVSBSPLIT_H
#define CVSBSPLIT_H

#include <QThread>
#include <QFile>

class CVSBSplit : public QThread
{
    Q_OBJECT

public:
    CVSBSplit(QObject* pParent);
    ~CVSBSplit();

    virtual void run();

public:
    void setFileName(const QString& strFileFullName);
    void stopSplit(bool bStop=true);

    void SetSplitParam(bool bMaxFileSize, unsigned nMaxFileSizeOrMsgNumber);

signals:
    void updatePrg(int nPrgVal);
    void updateList(const QString& strPrompt);
    void enableSplitBtn(bool bEnable);

public slots:

private:
    bool CreateVSBFile(int nIndex, QFile& vsbFile);

private:
    bool     m_bRunning;
    QString  m_strFileFullName;
    unsigned m_nMaxMsgNumber;
};

#endif // CVSBSPLIT_H
