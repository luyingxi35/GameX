#include "GameX/animation/manager.h"

#include "GameX/application/application.h"

namespace GameX::Animation {
Manager::Manager(Base::Renderer *renderer) : renderer_(renderer) {
  scene_ = std::make_unique<Base::Scene>(renderer_);
}

Manager::~Manager() {
  scene_.reset();
}

void Manager::Update(float delta_time) {
  ProcessCommandBufferQueue();
  for (auto &object : dynamic_objects_) {
    object->Update(delta_time);
  }
}

void Manager::PushCommand(const std::function<void(Manager *)> &command) {
  if (working_cmd_buffer_) {
    working_cmd_buffer_->commands.push(command);
  }
}

void Manager::RegisterObject(Object *object) {
  if (working_cmd_buffer_) {
    working_cmd_buffer_->new_objects.insert(object);
  }
}

void Manager::RegisterDynamicObject(DynamicObject *object) {
  if (working_cmd_buffer_) {
    working_cmd_buffer_->new_dynamic_objects.insert(object);
  }
}

void Manager::UnregisterObject(Object *object) {
  if (working_cmd_buffer_) {
    working_cmd_buffer_->delete_objects.insert(object);
  }
}

void Manager::UnregisterDynamicObject(DynamicObject *object) {
  dynamic_objects_.erase(object);
}

void Manager::ProcessCommandBufferQueue() {
  std::lock_guard<std::mutex> lock(cmd_buffer_queue_mutex_);

  while (!cmd_buffer_queue_.empty()) {
    auto &cmd_buffer = cmd_buffer_queue_.front();

    for (auto &object : cmd_buffer.new_objects) {
      objects_.insert(object);
    }

    for (auto &object : cmd_buffer.new_dynamic_objects) {
      dynamic_objects_.insert(object);
    }

    while (!cmd_buffer.commands.empty()) {
      cmd_buffer.commands.front()(this);
      cmd_buffer.commands.pop();
    }

    if (!cmd_buffer.delete_objects.empty()) {
      renderer_->App()->VkCore()->Device()->WaitIdle();
      for (auto &object : cmd_buffer.delete_objects) {
        objects_.erase(object);
        delete object;
      }
    }

    cmd_buffer_queue_.pop();
  }
}

}  // namespace GameX::Animation
