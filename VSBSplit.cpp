#include "VSBSplit.h"
#include <QFile>

const char gHead[10] = { 0x69, 0x63, 0x73, 0x62, 0x69, 0x6E, 0x02, 0x01, 0x00, 0x00 };

CVSBSplit::CVSBSplit(QObject* pParent)
{
    m_bRunning = true;
    m_nMaxMsgNumber = 0;

    if (pParent)
    {
        connect(this, SIGNAL(updatePrg(int)), pParent, SLOT(updatePrg(int)));
        connect(this, SIGNAL(updateList(const QString&)), pParent, SLOT(updateList(const QString&)));
        connect(this, SIGNAL(enableSplitBtn(bool)), pParent, SLOT(enableSplitBtn(bool)));
    }
}

CVSBSplit::~CVSBSplit()
{
    m_bRunning = false;
}

void CVSBSplit::run()
{
    m_bRunning = true;

    emit updatePrg(0);
    QFile vsbFile(m_strFileFullName);
    if (!vsbFile.open(QFile::ReadOnly))
    {
        emit updateList("Failed to open VSB File: " + m_strFileFullName);
        emit enableSplitBtn(true);
        return;
    }

    qint64 nTotalMsgCount = (vsbFile.size()-10)/64;

    char szHead[10] = {0};
    if (vsbFile.read(szHead, 10) != 10 ||
        memcmp(szHead, gHead, 10) != 0)
    {
        emit updateList("The Split file is not a vsb file or it is not a 0x102 format!");
        emit enableSplitBtn(true);
        return;
    }

    QFile splitVSBFile;
    int nIndex = 1;
    char szData[64] = {0};
    while(CreateVSBFile(nIndex++, splitVSBFile))
    {
        emit updateList("Split the data to the [" + QString::number(nIndex-1) + "] file, please wait...");

        for (unsigned i=0; i<m_nMaxMsgNumber; ++i)
        {
            if (!m_bRunning)
            {
                emit updateList("The split operation has been canceled!");
                emit enableSplitBtn(true);
                return;
            }

            if (vsbFile.read(szData, 64) != 64)
            {
                if (!vsbFile.atEnd())
                {
                    emit updateList("Failed to read the data from the source file!");
                    emit updateList("Failed Position: " + QString::number(vsbFile.pos()));
                    emit enableSplitBtn(true);
                    return;
                }
                else
                {
                    emit updateList("Success to split the file!");
                    emit updatePrg(100);
                    emit enableSplitBtn(true);
                    return;
                }
            }
            else
            {
                splitVSBFile.write(szData, 64);

                if (i%100000 == 0)
                {
                    emit updatePrg(((nIndex-2)*m_nMaxMsgNumber+i+1)*1.0 / nTotalMsgCount * 100);
                }
            }
        }

        emit updatePrg(((nIndex-1)*m_nMaxMsgNumber)*1.0 / nTotalMsgCount * 100);
    }

    emit updatePrg(100);

    vsbFile.close();
    splitVSBFile.close();
    emit enableSplitBtn(true);
}

void CVSBSplit::setFileName(const QString &strFileFullName)
{
    m_strFileFullName = strFileFullName;

    QFile file(strFileFullName);
    if (!file.exists())
    {
        emit updatePrg(0);
        emit updateList("VSB Split file: " + strFileFullName);
    }
    else
    {
        m_bRunning = true;
    }
}

void CVSBSplit::stopSplit(bool bStop)
{
    m_bRunning = !bStop;
}

void CVSBSplit::SetSplitParam(bool bMaxFileSize, unsigned nMaxFileSizeOrMsgNumber)
{
    if (bMaxFileSize)
    {
        m_nMaxMsgNumber = nMaxFileSizeOrMsgNumber * 1024 * 16;
    }
    else
    {
        m_nMaxMsgNumber = nMaxFileSizeOrMsgNumber;
    }
}

bool CVSBSplit::CreateVSBFile(int nIndex, QFile& vsbFile)
{
    vsbFile.close();

    QString strNewFileName = m_strFileFullName.mid(0, m_strFileFullName.lastIndexOf(".")) + "-";
    strNewFileName += QString::number(nIndex) + ".vsb";
    vsbFile.setFileName(strNewFileName);

    if (!vsbFile.open(QFile::WriteOnly))
    {
        emit updateList("Failed to create VSB File: " + strNewFileName);
        return false;
    }
    else
    {
        emit updateList("Success to create VSB File: " + strNewFileName);
    }

    vsbFile.write(gHead, 10);

    return true;
}
