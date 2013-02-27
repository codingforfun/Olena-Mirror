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

#include "generaloptions.hh"
#include "Preferences/generaloptions.ui.h"

GeneralOptions::GeneralOptions(QWidget *parent) :
        OptionWidget(parent),
        ui(new Ui::GeneralOptions)
{
    ui->setupUi(this);
    loadConfig();
}

GeneralOptions::~GeneralOptions()
{
    delete ui;
}

void GeneralOptions::loadConfig()
{
    Configs *const conf = Configs::getInstance();

    ui->saveXml->setChecked(conf->generalSaveXmlEnabled());
    ui->sameDir->setChecked(conf->generalSaveXmlSameDir());
    ui->customDir->setChecked(conf->generalSaveXmlCustomDir());
    ui->customDirValue->setText(conf->generalSaveXmlCustomDirPath());
}


void GeneralOptions::saveConfig()
{
    Configs *const conf = Configs::getInstance();

    conf->setGeneralSaveXmlEnabled(ui->saveXml->isChecked());
    conf->setGeneralSaveXmlSameDir(ui->sameDir->isChecked());
    conf->setGeneralSaveXmlCustomDir(ui->customDir->isChecked());
    conf->setGeneralSaveXmlCustomDirPath(ui->customDirValue->text());
}

void GeneralOptions::onCustomDirBrowseBtnClicked()
{
    QString dir = QFileDialog::getExistingDirectory(0, "Choose a directory");

    if (!dir.isEmpty())
        ui->customDirValue->setText(dir);
}
