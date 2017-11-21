#include "Dialog.h"
#include "ui_Dialog.h"
#include <QFileDialog>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    QRegExp regExp("[0-9]+");
    QRegExpValidator* pValidate = new QRegExpValidator(regExp, this);
    ui->m_pEditMaxFileSize->setValidator(pValidate);
    ui->m_pEditNumberMsgs->setValidator(pValidate);

    m_pVSBSplit = new CVSBSplit(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_m_pBtnBrowser_clicked()
{
    QString strFileName = QFileDialog::getOpenFileName(this,
                                                       tr("Select a VSB File"),
                                                       "",
                                                       tr("VSB Files (*.vsb)"));

    if (!strFileName.isNull())
    {
        ui->m_pEditFileFullName->setText(strFileName);
    }
    else
    {
        ui->m_pEditFileFullName->setText("");
    }
}

void Dialog::on_m_pRadioMaxSize_clicked(bool checked)
{
    ui->m_pEditMaxFileSize->setEnabled(checked);
    ui->m_pEditNumberMsgs->setEnabled(!checked);
}

void Dialog::on_m_pRadioNumberMsg_clicked(bool checked)
{
    ui->m_pEditMaxFileSize->setEnabled(!checked);
    ui->m_pEditNumberMsgs->setEnabled(checked);
}

void Dialog::on_m_pBtnSplit_clicked(bool checked)
{
    if (ui->m_pRadioMaxSize->isChecked())
    {
        m_pVSBSplit->SetSplitParam(true, ui->m_pEditMaxFileSize->text().toUInt());
    }
    else
    {
        m_pVSBSplit->SetSplitParam(false, ui->m_pEditNumberMsgs->text().toUInt());
    }

    if (checked)
    {
        m_pVSBSplit->setFileName(ui->m_pEditFileFullName->text());
        m_pVSBSplit->stopSplit();
        m_pVSBSplit->wait(500);

        m_pVSBSplit->start();
    }
    else
    {
        m_pVSBSplit->stopSplit();
    }
}

void Dialog::updatePrg(int nPrgVal)
{
    if (nPrgVal > 100)
    {
        nPrgVal = 100;
    }
    ui->m_pPrgPrompt->setValue(nPrgVal);
}

void Dialog::updateList(const QString &strPrompt)
{
    ui->m_pListPrompt->addItem(strPrompt);
}

void Dialog::enableSplitBtn(bool bEnable)
{
    ui->m_pBtnSplit->setChecked(bEnable);
    ui->m_pBtnSplit->toggle();
}
