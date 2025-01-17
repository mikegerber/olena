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


#include "image_scene.hh"
#include "image_region.hh"

ImageScene::ImageScene(QObject *parent)
  : QGraphicsScene(parent), selected_(0)
{
}

void
ImageScene::clear()
{
  selected_ = 0;
  QGraphicsScene::clear();
}

void
ImageScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
  QGraphicsScene::mousePressEvent(event);
  QList<QGraphicsItem *> items_list = items(event->scenePos()); // includes both ImageRegions and the picture.

  if (items_list.isEmpty())
    {
      if (selected_)
	{
	  selected_->deselect();
	  emit deselected();
	  selected_ = 0;
	}
    }

  // Selection is under the mouse click (at event->pos()).
  bool selection_is_clicked = items_list.contains(selected_);

  foreach(QGraphicsItem* elt, items_list)
    {
      ImageRegion* item = dynamic_cast<ImageRegion*>(elt);
      if (item)
	{
	  if (item != selected_)
	    {
	      if (selected_)
		{
		  if ( (item->boundingRect().intersects(selected_->boundingRect())))
		    {
		      int item_area = item->boundingRect().size().height() * item->boundingRect().size().width();
		      int selected_area = selected_->boundingRect().size().height() * selected_->boundingRect().size().width();

		      if (selected_area < item_area && selection_is_clicked)
			return;
		      else
			{
			  selected_->deselect();
			  emit deselected();
			  selected_ = 0;
			}
		    }
		  else
		    {
		      selected_->deselect();
		      emit deselected();
		      selected_ = 0;
		    }
		}
	      selected_ = item;
	      item->select();
	      emit selected(item->attr_id(), item->name());
	      return;
	    }
	}
      else
	{
	  if ( (selected_)
	       && (items_list.size() == 1) )// no ImageRegion, only the picture
	    {
	      selected_->deselect();
	      emit deselected();
	      selected_ = 0;
	    }
	}
    }
}

ImageScene::~ImageScene()
{
}
