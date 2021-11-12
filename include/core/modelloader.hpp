#pragma once 
#define TINYOBJLOADER_IMPLEMENTATION

#include "external/tinyobjloader/tiny_obj_loader.h"
#include <vector>
#include <iostream>

bool loadObj(const std::string &filename, std::vector<float> &vertices,
             std::vector<unsigned int> &indices, std::vector<float> &normals,
             std::vector<float> &uvs)
{
  tinyobj::ObjReader reader;
  std::cout << "test" << std::endl;
  if (!reader.ParseFromFile(filename))
  {
    if (!reader.Error().empty())
    {
      std::cerr << reader.Error();
    }
    return false;
  }

  if (!reader.Warning().empty())
  {
    std::cout << reader.Warning();
  }

  const auto &attrib = reader.GetAttrib();
  const auto &shapes = reader.GetShapes();

  vertices = attrib.vertices;
  if (attrib.normals.size() == attrib.vertices.size())
  {
    normals = attrib.normals;
  }

  if (attrib.texcoords.size() == (attrib.vertices.size() / 3) * 2)
  {
  for (int i = 0; i < attrib.texcoords.size(); i++)
  {
    uvs = attrib.texcoords;
  }
  }

  for (size_t s = 0; s < shapes.size(); ++s)
  {
    for (const auto &idx : shapes[s].mesh.indices)
    {
      indices.push_back(idx.vertex_index);
    }
  }

  return true;
}