# -*- c++ -*-
######################################################################
# Automatically generated by qmake (2.01a) Tue Feb 9 12:02:10 2010
######################################################################

TEMPLATE = app
TARGET =
DEPENDPATH += . src
INCLUDEPATH += . ../ ../shared $(OLN)/milena $(OLN)

DEFINES += NDEBUG MLN_INCLUDE_ONLY

# Input
HEADERS +=					\
 ../shared/src/image_viewer.hh			\
 ../shared/src/internal/interactive_scene.hh	\
 ../shared/src/crop_item.hh			\
  ../shared/src/browse_widget.hh		\
 src/load_page.hh				\
 src/load_widget.hh				\
 src/doc_type_widget.hh				\
 src/doc_type_page.hh				\
 src/crop_widget.hh				\
 src/crop_page.hh				\
 src/rotate_widget.hh				\
 src/rotate_page.hh				\
 src/preprocessing_widget.hh			\
 src/preprocessing_page.hh			\
 src/process_page.hh				\
 src/process_widget.hh				\
 src/runner.hh					\
 src/preprocessing_task.hh			\
 src/doc_type.hh				\
 src/main_window.hh


SOURCES +=					\
  ../shared/src/crop_item.cc			\
  ../shared/src/internal/interactive_scene.cc	\
  ../shared/src/browse_widget.cc		\
  src/main.cc					\
  src/main_window.cc				\
  src/doc_type_widget.cc				\
  src/doc_type_page.cc				\
  src/load_page.cc				\
  src/load_widget.cc				\
  src/crop_widget.cc				\
  src/crop_page.cc				\
  src/rotate_widget.cc				\
  src/rotate_page.cc				\
  src/preprocessing_widget.cc			\
  src/preprocessing_page.cc			\
  src/process_page.cc				\
  src/process_widget.cc				\
  src/mln_widgets.cc

FORMS +=					\
  ../shared/ui/browse_widget.ui			\
  ../shared/ui/crop_widget.ui			\
  ../shared/ui/rotate_widget.ui			\
  ../shared/ui/image_viewer.ui			\
  ui/preprocessing_widget.ui			\
  ui/doc_type_widget.ui				\
  ui/main_window.ui


RESOURCES += ../shared/shared.qrc wizard.qrc
