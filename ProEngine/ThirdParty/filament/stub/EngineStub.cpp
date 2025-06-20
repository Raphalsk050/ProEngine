#include <filament/Engine.h>
#include <utils/PrivateImplementation-impl.h>

namespace filament {

struct Engine::BuilderDetails {};

Engine::Builder::Builder() noexcept {}
Engine::Builder::Builder(Builder const&) noexcept {}
Engine::Builder::Builder(Builder&&) noexcept {}
Engine::Builder::~Builder() noexcept {}
Engine::Builder& Engine::Builder::operator=(Builder const&) noexcept { return *this; }
Engine::Builder& Engine::Builder::operator=(Builder&&) noexcept { return *this; }
Engine::Builder& Engine::Builder::backend(Backend) noexcept { return *this; }
Engine::Builder& Engine::Builder::platform(Platform*) noexcept { return *this; }
Engine::Builder& Engine::Builder::config(const Config*) noexcept { return *this; }
Engine::Builder& Engine::Builder::sharedContext(void*) noexcept { return *this; }
Engine* Engine::Builder::build() const {
    static Engine engine;
    return &engine;
}

void Engine::destroy(Engine* /*engine*/) {}
void Engine::destroy(Engine** engine) {
    if (engine) *engine = nullptr;
}

} // namespace filament
