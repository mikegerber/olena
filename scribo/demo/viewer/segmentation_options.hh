// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_DEMO_VIEWER_SEGMENTATION_OPTIONS_HH
# define SCRIBO_DEMO_VIEWER_SEGMENTATION_OPTIONS_HH

# include <QtGui>
# include <segmentation_options.ui.h>
# include "option_widget.hh"

class segmentation_options : public OptionWidget, private Ui::SegmentationOptions
{
  Q_OBJECT;

public:
  segmentation_options(QWidget *parent = 0);
  ~segmentation_options();

  void load_config();
  void save_config();

};

#endif // ! SCRIBO_DEMO_VIEWER_SEGMENTATION_OPTIONS_HH
