#include "GameX/renderer/texture.h"

#include "GameX/application/application.h"
#include "GameX/renderer/renderer.h"

namespace GameX {
Texture::Texture(Renderer *renderer, int width, int height) {
  Init(renderer, width, height);
}

void Texture::Init(GameX::Renderer *renderer, int width, int height) {
  image_ = std::make_unique<grassland::vulkan::Image>(
      renderer->App()->Core(), VK_FORMAT_R8G8B8A8_UNORM,
      VkExtent2D{static_cast<uint32_t>(width), static_cast<uint32_t>(height)});
}

}  // namespace GameX