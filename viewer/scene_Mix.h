#pragma once

#include "PolyLineDataReader.h"
#define ANARI_EXTENSION_UTILITY_IMPL
#include "anari/anari_cpp.hpp"
#include "anari/anari_cpp/ext/std.h"
#include <vtkm/io/VTKDataSetReader.h>
#include <vtkm/cont/CellSetSingleType.h>
#include <vtkm/cont/ArrayCopy.h>
#include <vtkm/cont/ColorTable.h>
#include <vtkm/cont/ColorTableSamples.h>
#include <iostream>
#include <string>
#include <array>
#include <fstream>
#include <vector>
#include <sstream>
#include <thread>
#include <chrono>
#include <ctime>
// 右脑皮层文件名
std::vector<std::string> rightHemisphereCorticalFiles = {
    "rh.bankssts.smoothed.obj",
    "rh.caudalanteriorcingulate.smoothed.obj",
    "rh.caudalmiddlefrontal.smoothed.obj",
    "rh.cuneus.smoothed.obj",
    "rh.entorhinal.smoothed.obj",
    "rh.frontalpole.smoothed.obj",
    "rh.fusiform.smoothed.obj",
    "rh.inferiorparietal.smoothed.obj",
    "rh.inferiortemporal.smoothed.obj",
    "rh.insula.smoothed.obj",
    "rh.isthmuscingulate.smoothed.obj",
    "rh.lateraloccipital.smoothed.obj",
    "rh.lateralorbitofrontal.smoothed.obj",
    "rh.lingual.smoothed.obj",
    "rh.medialorbitofrontal.smoothed.obj",
    "rh.middletemporal.smoothed.obj",
    "rh.paracentral.smoothed.obj",
    "rh.parahippocampal.smoothed.obj",
    "rh.parsopercularis.smoothed.obj",
    "rh.parsorbitalis.smoothed.obj",
    "rh.parstriangularis.smoothed.obj",
    "rh.pericalcarine.smoothed.obj",
    "rh.postcentral.smoothed.obj",
    "rh.posteriorcingulate.smoothed.obj",
    "rh.precentral.smoothed.obj",
    "rh.precuneus.smoothed.obj",
    "rh.rostralanteriorcingulate.smoothed.obj",
    "rh.rostralmiddlefrontal.smoothed.obj",
    "rh.superiorfrontal.smoothed.obj",
    "rh.superiorparietal.smoothed.obj",
    "rh.superiortemporal.smoothed.obj",
    "rh.supramarginal.smoothed.obj",
    "rh.temporalpole.smoothed.obj",
    "rh.transversetemporal.smoothed.obj",
};
// 右脑皮层文件名
std::vector<std::string> leftHemisphereCorticalFiles = {
    "lh.bankssts.smoothed.obj",
    "lh.caudalanteriorcingulate.smoothed.obj",
    "lh.caudalmiddlefrontal.smoothed.obj",
    "lh.cuneus.smoothed.obj",
    "lh.entorhinal.smoothed.obj",
    "lh.frontalpole.smoothed.obj",
    "lh.fusiform.smoothed.obj",
    "lh.inferiorparietal.smoothed.obj",
    "lh.inferiortemporal.smoothed.obj",
    "lh.insula.smoothed.obj",
    "lh.isthmuscingulate.smoothed.obj",
    "lh.lateraloccipital.smoothed.obj",
    "lh.lateralorbitofrontal.smoothed.obj",
    "lh.lingual.smoothed.obj",
    "lh.medialorbitofrontal.smoothed.obj",
    "lh.middletemporal.smoothed.obj",
    "lh.paracentral.smoothed.obj",
    "lh.parahippocampal.smoothed.obj",
    "lh.parsopercularis.smoothed.obj",
    "lh.parsorbitalis.smoothed.obj",
    "lh.parstriangularis.smoothed.obj",
    "lh.pericalcarine.smoothed.obj",
    "lh.postcentral.smoothed.obj",
    "lh.posteriorcingulate.smoothed.obj",
    "lh.precentral.smoothed.obj",
    "lh.precuneus.smoothed.obj",
    "lh.rostralanteriorcingulate.smoothed.obj",
    "lh.rostralmiddlefrontal.smoothed.obj",
    "lh.superiorfrontal.smoothed.obj",
    "lh.superiorparietal.smoothed.obj",
    "lh.superiortemporal.smoothed.obj",
    "lh.supramarginal.smoothed.obj",
    "lh.temporalpole.smoothed.obj",
    "lh.transversetemporal.smoothed.obj",
};
// 大脑皮层文件路径
std::string rhPath = "/home/fgao/code/brain_visualization/Surface/stl_files_smooth_transform/rh-obj/";
std::string lhPath = "/home/fgao/code/brain_visualization/Surface/stl_files_smooth_transform/lh-obj/";

// 创建场景
namespace anari
{
  namespace scenes
    {
      // 辅助函数：读取.obj文件的顶点、法线和索引
      void readMeshData(const std::string& filePath,
                        std::vector<std::array<float, 3>>& vertices,
                        std::vector<std::array<float, 3>>& normals,
                        std::vector<std::array<unsigned int, 3>>& indices) {
        std::ifstream fileStream(filePath);
        if (!fileStream.is_open()) {
          std::cerr << "Error opening file: " << filePath << std::endl;
          return;
        }

        std::string line;
        while (std::getline(fileStream, line)) {
          std::istringstream iss(line);
          std::string prefix;
          iss >> prefix;

          if (prefix == "v") {
            std::array<float, 3> vertex;
            iss >> vertex[0] >> vertex[1] >> vertex[2];
            vertices.push_back(vertex);
          } else if (prefix == "vn") {
            std::array<float, 3> normal;
            iss >> normal[0] >> normal[1] >> normal[2];
            normals.push_back(normal);
          } else if (prefix == "f") {
            std::array<unsigned int, 3> faceIndices;
            for (int i = 0; i < 3; ++i) {
              std::string vertexInfo;
              iss >> vertexInfo;
              size_t pos = vertexInfo.find("//");
              if (pos != std::string::npos) {
                std::string indexStr = vertexInfo.substr(0, pos);
                faceIndices[i] = static_cast<unsigned int>(std::stoi(indexStr) - 1);
              }
            }
            indices.push_back(faceIndices);
          }
        }
        fileStream.close();
      }


    inline anari::World CreateFiberTracks(anari::Device d, std::string filename1)
    {
      // 重命名
      using vec3 = std::array<float, 3>;
      using vec4 = std::array<float, 4>;
      using box3 = std::array<vec3, 2>;
      using vec3int = std::array<int, 3>;
      using Array1D_UINT32_VEC3 = std::vector<vec3int>;
      using vec3uint = std::array<unsigned int, 3>;
      using Array1D_Float32_VEC3 = std::vector<vec3>;

      // Load Dataset
      std::cout << std::endl;
      printf("start working start working");
      std::cout << std::endl;
      printf("loading ... %s", filename1.c_str());
      // 文件
      vtkm::io::PolyLineDataReader reader(filename1.c_str());
      static vtkm::cont::DataSet ds = reader.ReadDataSet();
      ds.PrintSummary(std::cout);
      std::cout << std::endl;
      // coords 为返回的三维坐标位置
      auto coords = ds.GetCoordinateSystem().GetData().AsArrayHandle<vtkm::cont::ArrayHandle<vtkm::Vec3f_32>>();
      
      // 获取曲线的偏移数组（curve_offsets），这些偏移数组用于定义曲线的段（segments）和顶点（vertices）之间的关系。
      auto curves = ds.GetCellSet().AsCellSet<vtkm::cont::CellSetExplicit<>>();
      // 访问对象为体cell,访问操作元素为点
      auto curve_offsets = curves.GetOffsetsArray(vtkm::TopologyElementTagCell(), vtkm::TopologyElementTagPoint());
      auto curve_offsets_reader = curve_offsets.ReadPortal();
      std::cout << "curve_offsets.GetNumberOfValues() Number is " << curve_offsets.GetNumberOfValues() << std::endl;

      // 读取DTI文件
      std::ifstream file("/home/fgao/code/brain_visualization/model/DTI_processed_ACT_5TTwmmask_seedgmwmi_1M_sift0.1M_S0.txt");
      
      // 错误处理
      if (!file.is_open())
      {
        std::cerr << "无法打开文件: " << std::endl;
        return anari::World();
      }
      std::vector<float> data;
      float value;

      while (file >> value)
      {
        data.push_back(value);
      }

      if (file.fail() && !file.eof())
      {
        std::cerr << "读取文件时发生错误。" << std::endl;
      }
      else if (file.eof())
      {
        std::cout << "文件读取完毕。" << std::endl;
      }
      file.close();

      std::cout << "the  Feature file  is already been readed" << std::endl;
      std::cout << "the  data size  is " << data.size() << std::endl;

      double scalar_length_max = 0;
      double scalar_length_min = std::numeric_limits<double>::max();
      uint32_t curve_length_max = 0;
      uint32_t curve_length_min = std::numeric_limits<uint32_t>::max();
      std::vector<uint32_t> curve_lengths(curve_offsets.GetNumberOfValues() - 1);
      // Curves are defined by a sequence of segments.
      // The i-th segment is defined by two points: vertex[prim[i]], vertex[prim[i]+1].
      // Thus if a curve has n+1 vertices, it has n segments (aka n primitives).

      // 获取神经纤维曲线的不同分段的起点和分段长度
      for (int i = 0; i < data.size(); i++)
      {
        scalar_length_max = std::max(scalar_length_max, static_cast<double>(data[i]));
        scalar_length_min = std::min(scalar_length_min, static_cast<double>(data[i]));
      }
      std::vector<uint32_t> segment_indices;
      for (int i = 1; i < curve_offsets.GetNumberOfValues(); i++)
      {
        auto offset_curr = curve_offsets_reader.Get(i - 1);
        auto offset_next = curve_offsets_reader.Get(i);
        auto num_segments = offset_next - offset_curr - 1;
        curve_lengths[i - 1] = offset_next - offset_curr;
        curve_length_max = std::max(curve_length_max, curve_lengths[i - 1]);
        curve_length_min = std::min(curve_length_min, curve_lengths[i - 1]);
        for (int j = 0; j < num_segments; j++)
        {
          segment_indices.push_back(offset_curr + j);
        }
      }
      std::cout << "scalar_length_max: " << scalar_length_max << std::endl;
      std::cout << "scalar_length_min : " << scalar_length_min << std::endl;
      std::cout << "curve_length_max: " << curve_length_max << std::endl;
      std::cout << "curve_length_min : " << curve_length_min << std::endl;

      // Color Mapping //
      // 配色方案 vtkm::cont::ColorTable::Preset::RainbowDesaturated
      vtkm::cont::ColorTable color_table(vtkm::cont::ColorTable::Preset::RainbowDesaturated);
      color_table.SetColorSpace(vtkm::ColorSpace::RGB);
      vtkm::cont::ColorTableSamplesRGB color_table_samples;
      // 采样了256种颜色
      color_table.Sample(256, color_table_samples);
      auto color_table_portal = color_table_samples.Samples.ReadPortal();
      std::vector<vec3> vertex_colors;
      vertex_colors.resize(coords.GetNumberOfValues());
      int primID = 0;

      std::cout << "coords.GetNumberOfValues:" << coords.GetNumberOfValues() << std::endl;
      for (int i = 0; i < coords.GetNumberOfValues(); i++)
      {
        // 进行颜色映射
        int v = ((data[i] - scalar_length_min) / (float)(scalar_length_max - scalar_length_min)) * 5 * color_table_samples.NumberOfSamples;
        auto c = color_table_portal.Get(std::min(v, color_table_samples.NumberOfSamples - 1));
        vertex_colors[i] = {
            c[0] / 255.f,
            c[1] / 255.f,
            c[2] / 255.f};
      }

      std::vector<anari::api::Surface *> surfaces;
      std::cout << "the fisrt dataset is already been processed" << std::endl;
      std::cout << "Start processing second dataset" << std::endl;
      // Add the full path programmatically:
      
      // 右脑处理
      std::string rightBasePath = "/home/fgao/code/brain_visualization/Surface/stl_files_smooth_transform/rh-obj/";
      for (const auto& file : rightHemisphereCorticalFiles) {
        std::string filePath = rightBasePath + file;
        std::vector<std::array<float, 3>> vertices, normals;
        std::vector<std::array<unsigned int, 3>> indices;
        readMeshData(filePath, vertices, normals, indices);

        auto surface = anari::newObject<anari::Surface>(d);
        auto mesh = anari::newObject<anari::Geometry>(d, "triangle");
        anari::setParameterArray1D(d, mesh, "vertex.position", vertices.data(), vertices.size());
        anari::setParameterArray1D(d, mesh, "vertex.normal", normals.data(), normals.size());
        anari::setParameterArray1D(d, mesh, "primitive.index", indices.data(), indices.size());
        anari::commitParameters(d, mesh);
        anari::setAndReleaseParameter(d, surface, "geometry", mesh);

        auto transMat = anari::newObject<anari::Material>(d, "physicallyBased");
        anari::setParameter(d, transMat, "alphaMode", "blend");
        // anari::setParameter(d, transMat, "baseColor", vec4{0.8f, 0.5f, 0.0f, 1.0f});

        anari::setParameter(d, transMat, "baseColor", vec4{0.978f, 0.538f, 0.538f, 1.0f});
        anari::setParameter(d, transMat, "opacity", 0.5f);
        anari::setParameter(d, transMat, "metallic", 0.4f);
        anari::setParameter(d, transMat, "roughness", 0.1f);
        anari::setParameter(d, transMat, "emissive", vec3{0.0f, 1.0f, 1.0f});
        anari::setParameter(d, transMat, "transmission", 1.5f);
        anari::setParameter(d, transMat, "thickness", 1.5f);
        anari::commitParameters(d, transMat);

        anari::setAndReleaseParameter(d, surface, "material", transMat);
        anari::commitParameters(d, surface);

        surfaces.push_back(surface);
      }

      // 左脑处理
      std::vector<std::array<float, 3>> colorList = {/* ... */}; // 保持原列表不变
      std::string leftBasePath = "/home/fgao/code/brain_visualization/Surface/stl_files_smooth_transform/lh-obj/";
      int colorIndex = 0;
      for (const auto& file : leftHemisphereCorticalFiles) {
        std::string filePath = leftBasePath + file;
        std::vector<std::array<float, 3>> vertices, normals;
        std::vector<std::array<unsigned int, 3>> indices;
        readMeshData(filePath, vertices, normals, indices);

        auto surface = anari::newObject<anari::Surface>(d);
        auto mesh = anari::newObject<anari::Geometry>(d, "triangle");
        anari::setParameterArray1D(d, mesh, "vertex.position", vertices.data(), vertices.size());
        anari::setParameterArray1D(d, mesh, "vertex.normal", normals.data(), normals.size());
        anari::setParameterArray1D(d, mesh, "primitive.index", indices.data(), indices.size());
        anari::commitParameters(d, mesh);
        anari::setAndReleaseParameter(d, surface, "geometry", mesh);

        auto matteMat = anari::newObject<anari::Material>(d, "matte");
        vec3 currentColor = colorList[colorIndex++]; // Ensure the index stays within bounds

        anari::setParameter(d, matteMat, "color", currentColor);
        anari::commitParameters(d, matteMat);
        anari::setAndReleaseParameter(d, surface, "material", matteMat);
        anari::commitParameters(d, surface);

        surfaces.push_back(surface);
      }
      auto world = anari::newObject<anari::World>(d);

      // Create and setup surface and mesh
      // 创建神经纤维曲线
      auto mesh1 = anari::newObject<anari::Geometry>(d, "curve");
      {
        vtkm::cont::Token token;
        auto *ptr = (vec3 *)coords.GetBuffers()[0].ReadPointerHost(token);
        anari::setParameterArray1D(d, mesh1, "vertex.position", ptr, coords.GetNumberOfValues());
      }

      anari::setParameterArray1D(d, mesh1, "vertex.color", vertex_colors.data(), vertex_colors.size());
      anari::setParameterArray1D(d, mesh1, "primitive.index", segment_indices.data(), segment_indices.size());
      anari::setParameter(d, mesh1, "radius", 0.1f);
      anari::commitParameters(d, mesh1);

      auto mat1 = anari::newObject<anari::Material>(d, "matte");
      anari::setParameter(d, mat1, "color", "color");
      anari::commitParameters(d, mat1);
      auto surface1 = anari::newObject<anari::Surface>(d);
      anari::setAndReleaseParameter(d, surface1, "geometry", mesh1);
      anari::setAndReleaseParameter(d, surface1, "material", mat1);
      anari::setParameter(d, surface1, "id", 2u);
      anari::commitParameters(d, surface1);
      surfaces.push_back(surface1);
      anari::setParameterArray1D(d, world, "surface", surfaces.data(), surfaces.size());
      anari::release(d, surface1);

      anari::Light light;

      // Fix this up with new check
      if (false /*anari::deviceImplements(d, "ANARI_KHR_AREA_LIGHTS")*/)
      {
        light = anari::newObject<anari::Light>(d, "quad");
        anari::setParameter(d, light, "color", vec3{0.38f, 0.351f, 0.183f});
        anari::setParameter(d, light, "intensity", 47.f);
        anari::setParameter(d, light, "position", vec3{-0.23f, 0.98f, -0.16f});
        anari::setParameter(d, light, "edge1", vec3{0.47f, 0.0f, 0.0f});
        anari::setParameter(d, light, "edge2", vec3{0.0f, 0.0f, 0.38f});
      }
      else
      {
        light = anari::newObject<anari::Light>(d, "directional");
        anari::setParameter(d, light, "direction", vec3{0.f, -1.0f, 1.f});
      }

      anari::commitParameters(d, light);

      anari::setAndReleaseParameter(
          d, world, "light", anari::newArray1D(d, &light));
      anari::release(d, light);

      anari::commitParameters(d, world);

      return world;
    }

  }
}
