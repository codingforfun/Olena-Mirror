// Copyright (C) 2013 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.

#include "preferencesdialog.hh"
#include "Preferences/preferencesdialog.ui.h"

PreferencesDialog::PreferencesDialog(QWidget *parent) :
        QDialog(parent, Qt::Dialog),
        ui(new Ui::PreferencesDialog)
{
    ui->setupUi(this);

    // We may want to remove this useless object in the ui file.
    delete ui->widget;

    connect(ui->optionList, SIGNAL(currentRowChanged(int)), this, SLOT(onOptionListCurrentRowChanged(int)));
    setAttribute(Qt::WA_DeleteOnClose);

    loadOptionList();
    ui->optionList->setCurrentRow(0);
}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}

void PreferencesDialog::loadOptionList()
{
    static const char *options[] = { "General", "Preprocessing", "Segmentation", "OCR", 0 };

    int i;
    for (i = 0; options[i]; ++i)
        ui->optionList->insertItem(i, options[i]);

    widgets.fill(0, i);
}

void PreferencesDialog::selectOptionWidget(int row)
{
    if(!widgets[row])
    {
        QWidget *widget = 0;

        switch(row)
        {
            case 0:
                widget = new GeneralOptions(this);
                break;

            case 1:
                widget = new PreprocessingOptions(this);
                break;

            case 2:
                widget = new SegmentationOptions(this);
                break;

            case 3:
                widget = new OcrOptions(this);
                break;
        }

        if(widget)
            widgets[row] = widget;
    }

    if(ui->horizontalLayout_2->count() == 2)
    {
        QWidget *current_widget = ui->horizontalLayout_2->itemAt(1)->widget();
        ui->horizontalLayout_2->removeWidget(current_widget);
        current_widget->hide();
    }

    ui->horizontalLayout_2->insertWidget(1, widgets[row]);
    widgets[row]->show();
}


void PreferencesDialog::accept()
{
    for (int i = 0; i < widgets.size(); ++i)
    {
        if (widgets[i])
        {
            static_cast<OptionWidget *>(widgets[i])->saveConfig();
            delete widgets[i];
        }
    }

    QDialog::accept();
}


void PreferencesDialog::reject()
{
    for (int i = 0; i < widgets.size(); ++i)
        delete widgets[i];

    QDialog::reject();
}


void PreferencesDialog::onOptionListCurrentRowChanged(int row)
{
  selectOptionWidget(row);
}
