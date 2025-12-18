#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/features/normal_3d.h>
#include <pcl/surface/gp3.h>
#include <pcl/io/vtk_io.h>

using namespace std;

// for bun*
//#define SEARCH_RADIUS 0.025
//#define MU 2.5
//#define MAX_NEAREST_NEIGHBORS 100
//#define MAX_SURFACE_ANGLE M_PI/4
//#define MIN_ANGLE M_PI/18
//#define MAX_ANGLE 2*M_PI/3

// for C.elegans Normal
#define DISTANCE_OF_PARTICLES 1.5E-6
#define WIDTH   68
#define HEIGHT  34

#define SEARCH_RADIUS DISTANCE_OF_PARTICLES * 3.0
#define MU 1.5  // Typical Value is 2.5-3 (or 1.5 for grids)
#define MAX_SURFACE_ANGLE M_PI/4  // 45 degrees
#define MIN_ANGLE M_PI/6  // 30 degrees
#define MAX_ANGLE 2*M_PI/3  // 120 degrees

#define MAX_NEAREST_NEIGHBORS 100  // Typical Value is 50-100
#define NORMAL_CONSISTENCY false  // Typically false

pcl::PointCloud<pcl::Normal>::Ptr surface_normals(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud)
{
  pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> ne;
  ne.setInputCloud(cloud);//法線の計算を行いたい点群を指定する

  pcl::search::KdTree<pcl::PointXYZ>::Ptr tree (new pcl::search::KdTree<pcl::PointXYZ> ());//KDTREEを作る
  ne.setSearchMethod(tree);//検索方法にKDTREEを指定する

  pcl::PointCloud<pcl::Normal>::Ptr cloud_normals (new pcl::PointCloud<pcl::Normal>);//法線情報を入れる変数

  ne.setRadiusSearch(SEARCH_RADIUS);//検索する半径を指定する

  ne.compute(*cloud_normals);//法線情報の出力先を指定する

  return cloud_normals;
}

pcl::PolygonMesh Generate_mesh(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud)
{
  pcl::PointCloud<pcl::Normal>::Ptr cloud_normals (new pcl::PointCloud<pcl::Normal> ());
  cloud_normals = surface_normals(cloud);

  pcl::PointCloud<pcl::PointNormal>::Ptr cloud_with_normals (new pcl::PointCloud<pcl::PointNormal>);
  pcl::concatenateFields (*cloud, *cloud_normals, *cloud_with_normals);

  pcl::search::KdTree<pcl::PointNormal>::Ptr tree2 (new pcl::search::KdTree<pcl::PointNormal>);
  tree2->setInputCloud (cloud_with_normals);

  pcl::GreedyProjectionTriangulation<pcl::PointNormal> gp3;
  pcl::PolygonMesh triangles;

  gp3.setSearchRadius (SEARCH_RADIUS);

  gp3.setMu(MU);
  gp3.setMaximumNearestNeighbors(MAX_NEAREST_NEIGHBORS);
  gp3.setMaximumSurfaceAngle(MAX_SURFACE_ANGLE); 
  gp3.setMinimumAngle(MIN_ANGLE);
  gp3.setMaximumAngle(MAX_ANGLE);
  gp3.setNormalConsistency(NORMAL_CONSISTENCY);

  gp3.setInputCloud (cloud_with_normals);
  gp3.setSearchMethod (tree2);
  gp3.reconstruct (triangles);

  std::vector<int> parts = gp3.getPartIDs();
  std::vector<int> states = gp3.getPointStates();

  return triangles;
}

int main(int argc, char* argv[]){
  // input "point.dat" File and convert into PCD file
  pcl::PointCloud<pcl::PointXYZ> dat;
  ifstream ifs("point.dat");
  if(!ifs){
    cerr << "Input Error";
    return 1;
  }

  int i = 0, j;
  double temp[3];
  string str;

  dat.width  = WIDTH;
  dat.height = HEIGHT;
  dat.is_dense = false;
  dat.points.resize(dat.width * dat.height);

  while(getline(ifs, str)){
    string token;
    istringstream stream(str);

    j = 0;
    while(getline(stream, token, ',')){
      temp[j] = stod(token);
      j++;
    }
    dat.points[i].x = temp[0];
    dat.points[i].y = temp[1];
    dat.points[i].z = temp[2];

    i++;
  }
  
  pcl::io::savePCDFileASCII("point.pcd", dat);
  cout << "Saved " << dat.points.size() << " data points to point.pcd." << endl; 

  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);
  cout << "input PCD File" << endl;
  pcl::io::loadPCDFile ("point.pcd", *cloud);
  cout << "input finished" << endl;

  std::stringstream Filename;
  std::string name = "point";
  name.erase(name.length() - 4);
  Filename << name << "_mesh.vtk";
  pcl::io::saveVTKFile(Filename.str(), Generate_mesh(cloud));

  return (0);
}
