#include <benchmark/benchmark.h>
#include <pcl/common/centroid.h>
#include <pcl/common/common.h>
#include <pcl/filters/conditional_removal.h>
#include <pcl/io/pcd_io.h>
#include <pcl/pcl_base.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>

using namespace pcl;

const char filename[] = "test_pcd.pcd";

PointCloud<pcl::PointXYZ>::Ptr LoadFromFile(const char *filename)
{
  PointCloud<pcl::PointXYZ>::Ptr cloud_in(new PointCloud<pcl::PointXYZ>);
  if (io::loadPCDFile<pcl::PointXYZ>(filename, *cloud_in) == -1)  //* load the file
  {
    std::cerr << "Couldn't read file " << filename << std::endl;
    throw std::runtime_error("file not found)");
  }
  return cloud_in;
}

static void PCL_Filter(benchmark::State & state)
{
  auto cloud = LoadFromFile(filename);

  PointXYZ minPt, maxPt;
  pcl::getMinMax3D(*cloud, minPt, maxPt);
  const float mid_point_x = (maxPt.x+minPt.x)*0.5 ;
  const float mid_point_y = (maxPt.y+minPt.y)*0.5 ;

  PointCloud<PointXYZ>::Ptr outCloud(new PointCloud<PointXYZ>);

  for (auto _ : state) {
    outCloud->clear();
    pcl::ConditionalRemoval<PointXYZ> range_filt;

    auto range_cond   = boost::make_shared<pcl::ConditionOr<PointXYZ>>();
    auto comparison_x = boost::make_shared<FieldComparison<PointXYZ>>("x", ComparisonOps::GT, mid_point_x);
    auto comparison_y = boost::make_shared<FieldComparison<PointXYZ>>("y", ComparisonOps::GT, mid_point_y);
    range_cond->addComparison(comparison_x);
    range_cond->addComparison(comparison_y);

    range_filt.setInputCloud(cloud);
    range_filt.setCondition(range_cond);
    range_filt.filter(*outCloud);
  }
}

static void Naive_Filter(benchmark::State & state)
{
  auto cloud = LoadFromFile(filename);

  PointXYZ minPt, maxPt;
  pcl::getMinMax3D(*cloud, minPt, maxPt);
  const float mid_point_x = (maxPt.x+minPt.x)*0.5 ;
  const float mid_point_y = (maxPt.y+minPt.y)*0.5 ;

  PointCloud<PointXYZ>::Ptr outCloud(new PointCloud<PointXYZ>);

  for (auto _ : state) {
    outCloud->clear();
    for (const auto& point: cloud->points) {
      if( point.x > mid_point_x && point.y > mid_point_y ){
        outCloud->push_back( point );
      }
    }
  }
}

BENCHMARK(PCL_Filter);
BENCHMARK(Naive_Filter);

//----------------------------------------------------------------
template <typename PointT>
class GenericCondition : public ConditionBase<PointT>
{
public:
  typedef boost::shared_ptr<GenericCondition<PointT>> Ptr;
  typedef boost::shared_ptr<const GenericCondition<PointT>> ConstPtr;
  typedef std::function<bool(const PointT&)> FunctorT;

  GenericCondition(FunctorT evaluator): ConditionBase<PointT>(),_evaluator(evaluator) {}

  virtual bool evaluate (const PointT &point) const {
    return _evaluator(point);
  }
private:
  FunctorT _evaluator;
};

static void PCL_Filter_Generic(benchmark::State & state)
{
  auto cloud = LoadFromFile(filename);

  PointXYZ minPt, maxPt;
  pcl::getMinMax3D(*cloud, minPt, maxPt);
  const float mid_point_x = (maxPt.x+minPt.x)*0.5 ;
  const float mid_point_y = (maxPt.y+minPt.y)*0.5 ;

  PointCloud<PointXYZ>::Ptr outCloud(new PointCloud<PointXYZ>);

  for (auto _ : state) {
    outCloud->clear();
    pcl::ConditionalRemoval<PointXYZ> range_filt;

    auto range_cond = boost::make_shared<GenericCondition<PointXYZ>>(
      [=](const PointXYZ& point){
        return point.x > mid_point_x && point.y > mid_point_y;
      });

    range_filt.setInputCloud(cloud);
    range_filt.setCondition(range_cond);
    range_filt.filter(*outCloud);
  }
}

BENCHMARK(PCL_Filter_Generic);

BENCHMARK_MAIN();
