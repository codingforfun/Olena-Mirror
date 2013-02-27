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

#include "segmentationoptions.hh"
#include "Preferences/segmentationoptions.ui.h"

SegmentationOptions::SegmentationOptions(QWidget *parent) :
        OptionWidget(parent),
        ui(new Ui::SegmentationOptions)
{
    ui->setupUi(this);

    ui->find_sepsCbox->insertItem(Separator::Lines, "Lines");
    ui->find_sepsCbox->insertItem(Separator::Whitespaces, "Whitespaces");
    ui->find_sepsCbox->insertItem(Separator::Both, "Lines and whitespaces");

    loadConfig();
}

SegmentationOptions::~SegmentationOptions()
{
    delete ui;
}

void SegmentationOptions::loadConfig()
{
    Configs *const conf = Configs::getInstance();
    ui->find_sepsCbox->setCurrentIndex(conf->segmentationFindSeps());
}


void SegmentationOptions::saveConfig()
{
    Configs *const conf = Configs::getInstance();
    conf->setSegmentationFindSeps(ui->find_sepsCbox->currentIndex());
}
