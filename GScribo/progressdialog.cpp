#include "progressdialog.h"

ProgressDialog::ProgressDialog(QWidget *parent, Qt::WindowFlags flags):
        QProgressDialog(parent, flags)
{
    setModal(true);
    setAutoClose(false);
    setCancelButton(0);
    hide();
}

void ProgressDialog::run()
{
    setValue(value()+1);
}

void ProgressDialog::reset()
{
    setValue(0);
    setLabelText("");
    show();
}
