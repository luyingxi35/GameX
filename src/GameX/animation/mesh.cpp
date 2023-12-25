#include "GameX/animation/mesh.h"

namespace GameX::Animation {

std::vector<Base::Vertex> &Mesh::Vertices() {
  return mesh_.Vertices();
}

const std::vector<Base::Vertex> &Mesh::Vertices() const {
  return mesh_.Vertices();
}

std::vector<uint32_t> &Mesh::Indices() {
  return mesh_.Indices();
}

const std::vector<uint32_t> &Mesh::Indices() const {
  return mesh_.Indices();
}

}  // namespace GameX::Animation