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
// 右脑皮层位置
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
// 右脑皮层位置
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
namespace anari
{
  namespace scenes
    {

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

      // 读取文件
      std::ifstream file("/home/fgao/code/brain_visualization/model/DTI_processed_ACT_5TTwmmask_seedgmwmi_1M_sift0.1M_S0.txt");

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

      std::string basePath = "/home/fgao/code/brain_visualization/Surface/stl_files_smooth_transform/rh-obj/";
      for (auto file : rightHemisphereCorticalFiles)
      {
        Array1D_Float32_VEC3 vertices;
        Array1D_Float32_VEC3 normals;
        Array1D_UINT32_VEC3 triangleIndices;

        auto surface2 = anari::newObject<anari::Surface>(d);

        std::string filePath = basePath + file; // Prepend base path to each file name
        std::cout << "211:: File name  is " << filePath << std::endl;
        std::ifstream fileStream(filePath);
        if (!fileStream.is_open())
        {
          std::cerr << "Error: Could not open file. Check the file path and permissions." << std::endl;
        }
        std::string line;
        while (std::getline(fileStream, line))
        {
          std::istringstream iss(line);
          std::string prefix;
          iss >> prefix;
          if (prefix == "v")
          {
            vec3 vertex;
            iss >> vertex[0] >> vertex[1] >> vertex[2];
            vertices.push_back(vertex);
          }
          else if (prefix == "vn")
          {
            vec3 normal;
            iss >> normal[0] >> normal[1] >> normal[2];
            normals.push_back(normal);
          }
          else if (prefix == "f")
          {
            std::array<int, 3> vertexIndices;
            for (int i = 0; i < 3; ++i)
            {
              std::string vertexInfo;
              iss >> vertexInfo;
              size_t pos = vertexInfo.find("//");
              if (pos != std::string::npos)
              {
                vertexIndices[i] = static_cast<uint32_t>(std::stoi(vertexInfo.substr(0, pos)) - 1);
              }
            }
            triangleIndices.push_back(vertexIndices);
          }
        }

        fileStream.close();

        vec3 *vertexPtr = vertices.data();
        vec3 *normalPtr = normals.data();
        vec3uint *indexPtr = reinterpret_cast<vec3uint *>(triangleIndices.data());

        auto mesh2 = anari::newObject<anari::Geometry>(d, "triangle");
        {
          anari::setParameterArray1D(d, mesh2, "vertex.position", vertexPtr, vertices.size());
          anari::setParameterArray1D(d, mesh2, "vertex.normal", normalPtr, normals.size());
          anari::setParameterArray1D(d, mesh2, "primitive.index", indexPtr, triangleIndices.size());
        }

        anari::commitParameters(d, mesh2);
        anari::setAndReleaseParameter(d, surface2, "geometry", mesh2);

        auto mat2 = anari::newObject<anari::Material>(d, "physicallyBased");
        anari::setParameter(d, mat2, "alphaMode", "blend");
        // anari::setParameter(d, mat2, "baseColor", vec4{0.8f, 0.5f, 0.0f, 1.0f});

        anari::setParameter(d, mat2, "baseColor", vec4{0.978f, 0.538f, 0.538f, 1.0f});
        anari::setParameter(d, mat2, "opacity", 0.5f);
        anari::setParameter(d, mat2, "metallic", 0.4f);
        anari::setParameter(d, mat2, "roughness", 0.1f);
        anari::setParameter(d, mat2, "emissive", vec3{0.0f, 1.0f, 1.0f});
        anari::setParameter(d, mat2, "transmission", 1.5f);
        anari::setParameter(d, mat2, "thickness", 1.5f);
        anari::commitParameters(d, mat2);

        anari::setAndReleaseParameter(d, surface2, "material", mat2);
        anari::setParameter(d, surface2, "id", 2u);
        anari::commitParameters(d, surface2);
        surfaces.push_back(surface2);
      }

      // 脑部区域颜色
      std::vector<vec3> colorList = {
          vec3{0.9608f, 0.6353f, 0.0863f}, // Example color 1 (Orange)
          vec3{0.1f, 0.3f, 1.0f},          // Example color 2 (Blue)
          vec3{0.978f, 0.538f, 0.538f},    // Example color 3 (Pink)
          vec3{0.839f, 0.773f, 0.706f},    // Example color 4 (Light Beige)
          vec3{0.855f, 0.788f, 0.557f},    // Example color 5 (Soft Yellow)
          vec3{1.0f, 0.84f, 0.0f},         // Example color 6 (Bright Yellow)
          vec3{0.578f, 0.447f, 0.698f},    // Example color 7 (Purple)
          vec3{0.125f, 0.654f, 0.525f},    // Example color 8 (Teal)
          vec3{0.925f, 0.474f, 0.169f},    // Example color 9 (Red-Orange)
          vec3{0.486f, 0.741f, 0.180f},    // Example color 10 (Lime Green)
          vec3{0.134f, 0.418f, 0.815f},    // Example color 11 (Sky Blue)
          vec3{0.870f, 0.137f, 0.227f},    // Example color 12 (Crimson)
          vec3{0.217f, 0.559f, 0.698f},    // Example color 13 (Light Blue)
          vec3{0.705f, 0.209f, 0.360f},    // Example color 14 (Deep Red)
          vec3{0.322f, 0.788f, 0.149f},    // Example color 15 (Green)
          vec3{0.878f, 0.929f, 0.196f},    // Example color 16 (Yellow-Green)
          vec3{0.601f, 0.804f, 0.180f},    // Example color 17 (Chartreuse)
          vec3{0.575f, 0.284f, 0.189f},    // Example color 18 (Rust)
          vec3{0.432f, 0.604f, 0.418f},    // Example color 19 (Olive Green)
          vec3{0.293f, 0.424f, 0.592f},    // Example color 20 (Dark Blue)
          vec3{0.666f, 0.435f, 0.276f},    // Example color 21 (Carmine)
          vec3{0.953f, 0.872f, 0.523f},    // Example color 22 (Goldenrod)
          vec3{0.836f, 0.524f, 0.298f},    // Example color 23 (Terra Cotta)
          vec3{0.320f, 0.550f, 0.482f},    // Example color 24 (Mint Green)
          vec3{0.528f, 0.184f, 0.407f},    // Example color 25 (Magenta)
          vec3{0.455f, 0.687f, 0.474f},    // Example color 26 (Sea Green)
          vec3{0.937f, 0.847f, 0.051f},    // Example color 27 (Mustard)
          vec3{0.803f, 0.537f, 0.235f},    // Example color 28 (Burnt Orange)
          vec3{0.421f, 0.703f, 0.933f},    // Example color 29 (Light Aqua)
          vec3{0.738f, 0.441f, 0.579f},    // Example color 30 (Rose Red)
          vec3{0.257f, 0.514f, 0.713f},    // Example color 31 (Steel Blue)
          vec3{0.804f, 0.765f, 0.334f},    // Example color 32 (Olive Yellow)
          vec3{0.976f, 0.663f, 0.443f},    // Example color 33 (Peach)
      };
      int i = 0;
      basePath = "/home/fgao/code/brain_visualization/Surface/stl_files_smooth_transform/lh-obj/";
      for (auto file : leftHemisphereCorticalFiles)
      {
        Array1D_Float32_VEC3 vertices;
        Array1D_Float32_VEC3 normals;
        Array1D_UINT32_VEC3 triangleIndices;

        auto surface2 = anari::newObject<anari::Surface>(d);

        std::string filePath = basePath + file; // Prepend base path to each file name
        std::cout << "327:: File name is " << filePath << std::endl;
        std::ifstream fileStream(filePath);
        if (!fileStream.is_open())
        {
          std::cerr << "Error: Could not open file. Check the file path and permissions." << std::endl;
        }
        std::string line;
        while (std::getline(fileStream, line))
        {
          std::istringstream iss(line);
          std::string prefix;
          iss >> prefix;
          if (prefix == "v")
          {
            vec3 vertex;
            iss >> vertex[0] >> vertex[1] >> vertex[2];
            vertices.push_back(vertex);
          }
          else if (prefix == "vn")
          {
            vec3 normal;
            iss >> normal[0] >> normal[1] >> normal[2];
            normals.push_back(normal);
          }
          else if (prefix == "f")
          {
            std::array<int, 3> vertexIndices;
            for (int i = 0; i < 3; ++i)
            {
              std::string vertexInfo;
              iss >> vertexInfo;
              size_t pos = vertexInfo.find("//");
              if (pos != std::string::npos)
              {
                vertexIndices[i] = static_cast<uint32_t>(std::stoi(vertexInfo.substr(0, pos)) - 1);
              }
            }
            triangleIndices.push_back(vertexIndices);
          }
        }

        fileStream.close();
        vec3 *vertexPtr = vertices.data();
        vec3 *normalPtr = normals.data();
        vec3uint *indexPtr = reinterpret_cast<vec3uint *>(triangleIndices.data());

        auto mesh2 = anari::newObject<anari::Geometry>(d, "triangle");
        {
          anari::setParameterArray1D(d, mesh2, "vertex.position", vertexPtr, vertices.size());
          anari::setParameterArray1D(d, mesh2, "vertex.normal", normalPtr, normals.size());
          anari::setParameterArray1D(d, mesh2, "primitive.index", indexPtr, triangleIndices.size());
        }

        anari::commitParameters(d, mesh2);
        anari::setAndReleaseParameter(d, surface2, "geometry", mesh2);

        auto mat2 = anari::newObject<anari::Material>(d, "matte");
        vec3 currentColor = colorList[i]; // Ensure the index stays within bounds

        anari::setParameter(d, mat2, "color", currentColor);
        anari::commitParameters(d, mat2);
        anari::setAndReleaseParameter(d, surface2, "material", mat2);
        anari::setParameter(d, surface2, "id", 2u);
        anari::commitParameters(d, surface2);
        surfaces.push_back(surface2);
        i++;
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
      anari::setParameter(d, world, "id", 3u);
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
