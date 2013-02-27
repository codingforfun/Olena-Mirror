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

#include "ocroptions.hh"
#include "Preferences/ocroptions.ui.h"

static const char *language[][2] =
{
    { "English", "eng" },
    { "French", "fra" },
    { 0, 0 }
};

OcrOptions::OcrOptions(QWidget *parent) :
        OptionWidget(parent),
        ui(new Ui::OcrOptions)
{
    ui->setupUi(this);

    for (unsigned i = 0; language[i][0]; ++i)
        ui->ocr_language->insertItem(i, language[i][0]);

    loadConfig();
}

OcrOptions::~OcrOptions()
{
    delete ui;
}

int OcrOptions::findIndex(const QString& lang)
{
    for (unsigned i = 0; language[i][0]; ++i)
        if (lang == language[i][1])
            return i;

    return 0;
}


void OcrOptions::loadConfig()
{
    Configs *const conf = Configs::getInstance();

    ui->enable_ocr->setChecked(conf->ocrEnabled());
    ui->ocr_language->setCurrentIndex(findIndex(conf->ocrLanguage()));
}


void OcrOptions::saveConfig()
{
    Configs *const conf = Configs::getInstance();

    conf->setOcrEnabled(ui->enable_ocr->isChecked());
    conf->setOcrLanguage(language[ui->ocr_language->currentIndex()][1]);
}
