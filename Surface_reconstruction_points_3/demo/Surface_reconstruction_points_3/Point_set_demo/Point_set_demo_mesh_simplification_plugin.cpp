#include "Point_set_demo_plugin_helper.h"
#include "Point_set_demo_plugin_interface.h"

#include "Scene_polyhedron_item.h"
#include "Point_set_demo_types.h"

#include <QApplication>
#include <QMainWindow>
#include <QInputDialog>
#include <QTime>

#include <CGAL/Surface_mesh_simplification/HalfedgeGraph_Polyhedron_3.h>
#include <CGAL/Surface_mesh_simplification/edge_collapse.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Count_stop_predicate.h>

class Point_set_demo_mesh_simplification_plugin : 
  public QObject,
  public Point_set_demo_plugin_helper
{
  Q_OBJECT
  Q_INTERFACES(Point_set_demo_plugin_interface);

public:
  // used by Point_set_demo_plugin_helper
  QStringList actionsNames() const {
    return QStringList() << "actionSimplify";
  }

public slots:
  void on_actionSimplify_triggered();

}; // end Point_set_demo_mesh_simplification_plugin

void Point_set_demo_mesh_simplification_plugin::on_actionSimplify_triggered()
{
  const Scene_interface::Item_id index = scene->mainSelectionIndex();
  
  Scene_polyhedron_item* item = 
    qobject_cast<Scene_polyhedron_item*>(scene->item(index));

  if(item)
  {
    Polyhedron* pMesh = item->polyhedron();

    // get option (#edges)
    bool ok;
    const unsigned int nb_edges = 
      QInputDialog::getInteger(mw, tr("Stop condition"),
      tr("Number of edges:"),
      pMesh->size_of_halfedges () / 4, // default value: current #edges / 2 
      3, // min = one triangle
      pMesh->size_of_halfedges(), // max #edges
      1, // step for the spinbox
      &ok);

    // check user cancellation
    if(!ok)
      return;

    // simplify
    QTime time;
    time.start();
    std::cout << "Simplify...";
    QApplication::setOverrideCursor(Qt::WaitCursor);
    namespace SMS = CGAL::Surface_mesh_simplification;
    SMS::Count_stop_predicate< Polyhedron > stop(nb_edges); // target #edges
    SMS::edge_collapse( *pMesh, stop,
      CGAL::vertex_index_map(boost::get(CGAL::vertex_external_index,*pMesh))
      .edge_index_map(boost::get(CGAL::edge_external_index,*pMesh)));
    std::cout << "ok (" << time.elapsed() << " ms, " 
      << pMesh->size_of_halfedges() / 2 << " edges)" << std::endl;

    // update scene
    scene->itemChanged(index);
    QApplication::restoreOverrideCursor();
  }
}

Q_EXPORT_PLUGIN2(Point_set_demo_mesh_simplification_plugin, Point_set_demo_mesh_simplification_plugin);

#include "Point_set_demo_mesh_simplification_plugin.moc"
