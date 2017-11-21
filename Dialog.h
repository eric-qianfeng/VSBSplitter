#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <VSBSplit.h>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void on_m_pBtnBrowser_clicked();

    void on_m_pRadioMaxSize_clicked(bool checked);

    void on_m_pRadioNumberMsg_clicked(bool checked);

    void on_m_pBtnSplit_clicked(bool checked);

public slots:
    void updatePrg(int nPrgVal);
    void updateList(const QString& strPrompt);
    void enableSplitBtn(bool bEnable);

private:
    Ui::Dialog *ui;

    CVSBSplit*  m_pVSBSplit;
};

#endif // DIALOG_H
