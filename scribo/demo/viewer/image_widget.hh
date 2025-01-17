// Copyright (C) 2010, 2011 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef SCRIBO_DEMO_VIEWER_IMAGE_WIDGET_HH
# define SCRIBO_DEMO_VIEWER_IMAGE_WIDGET_HH

# include <QtGui>
# include "image_view.hh"

class ImageView;

class ImageWidget
  : public QWidget
{
  Q_OBJECT

public:
  ImageWidget(QGraphicsScene* scene);
  ~ImageWidget();

  ImageView * view() const;

public slots:
  void update();

signals:
  void scaleUpdated(qreal scale);

private:
  ImageView* view_;
};

#endif // ! SCRIBO_DEMO_VIEWER_IMAGE_WIDGET_HH
