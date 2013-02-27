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

#include "preprocessingoptions.hh"
#include "Preferences/preprocessingoptions.ui.h"

using namespace scribo::toolchain::internal;

PreprocessingOptions::PreprocessingOptions(QWidget *parent) :
        OptionWidget(parent),
        ui(new Ui::PreprocessingOptions)
{
    ui->setupUi(this);

    ui->bin_algoCbox->insertItem(Convert, "Violent convert");
    ui->bin_algoCbox->insertItem(Sauvola, "Local threshold");
    ui->bin_algoCbox->insertItem(SauvolaMs, "Local threshold multiscale");

    loadConfig();
}

PreprocessingOptions::~PreprocessingOptions()
{
    delete ui;
}

void PreprocessingOptions::loadConfig()
{
    Configs *const conf = Configs::getInstance();

    ui->subsampleCb->setChecked(conf->preprocessingSubsample());
    ui->remove_bgCb->setChecked(conf->preprocessingRemoveBg());
    ui->deskewCb->setChecked(conf->preprocessingDeskew());
    ui->remove_noiseCb->setChecked(conf->preprocessingRemoveNoise());
    ui->bin_algoCbox->setCurrentIndex(conf->preprocessingBinAlgo());
}


void PreprocessingOptions::saveConfig()
{
    Configs *const conf = Configs::getInstance();

    conf->setPreprocessingSubsample(ui->subsampleCb->isChecked());
    conf->setPreprocessingRemoveBg(ui->remove_bgCb->isChecked());
    conf->setPreprocessingDeskew(ui->deskewCb->isChecked());
    conf->setPreprocessingRemoveNoise(ui->remove_noiseCb->isChecked());
    conf->setPreprocessingBinAlgo(ui->bin_algoCbox->currentIndex());
}
