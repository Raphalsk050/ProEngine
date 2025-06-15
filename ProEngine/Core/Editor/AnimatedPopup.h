// AnimatedPopup.h
#pragma once
#include <imgui.h>
#include <string>
#include <functional>
#include <chrono>
#include <cmath>

namespace ProEngine
{
    // Tipos de animação
    enum class PopupAnimation
    {
        FadeIn,
        SlideDown,
        SlideUp,
        SlideLeft,
        SlideRight,
        ScaleUp,
        ScaleDown,
        Bounce,
        Elastic,
        Rotate
    };

    // Funções de easing
    namespace Easing
    {
        inline float Linear(float t) { return t; }
        
        inline float EaseInQuad(float t) { return t * t; }
        inline float EaseOutQuad(float t) { return t * (2.0f - t); }
        inline float EaseInOutQuad(float t) 
        { 
            return t < 0.5f ? 2.0f * t * t : -1.0f + (4.0f - 2.0f * t) * t; 
        }
        
        inline float EaseInCubic(float t) { return t * t * t; }
        inline float EaseOutCubic(float t) { return (--t) * t * t + 1.0f; }
        inline float EaseInOutCubic(float t) 
        { 
            return t < 0.5f ? 4.0f * t * t * t : (t - 1.0f) * (2.0f * t - 2.0f) * (2.0f * t - 2.0f) + 1.0f; 
        }
        
        inline float EaseInElastic(float t) 
        {
            if (t == 0.0f || t == 1.0f) return t;
            float p = 0.3f;
            float s = p / 4.0f;
            return -std::pow(2.0f, 10.0f * (t - 1.0f)) * std::sin((t - 1.0f - s) * (2.0f * IM_PI) / p);
        }
        
        inline float EaseOutElastic(float t) 
        {
            if (t == 0.0f || t == 1.0f) return t;
            float p = 0.3f;
            float s = p / 4.0f;
            return std::pow(2.0f, -10.0f * t) * std::sin((t - s) * (2.0f * IM_PI) / p) + 1.0f;
        }
        
        inline float EaseOutBounce(float t) 
        {
            if (t < 1.0f / 2.75f) {
                return 7.5625f * t * t;
            } else if (t < 2.0f / 2.75f) {
                t -= 1.5f / 2.75f;
                return 7.5625f * t * t + 0.75f;
            } else if (t < 2.5f / 2.75f) {
                t -= 2.25f / 2.75f;
                return 7.5625f * t * t + 0.9375f;
            } else {
                t -= 2.625f / 2.75f;
                return 7.5625f * t * t + 0.984375f;
            }
        }
        
        inline float EaseInBounce(float t) 
        {
            return 1.0f - EaseOutBounce(1.0f - t);
        }
    }

    class AnimatedPopup
    {
    public:
        AnimatedPopup(const std::string& id = "##AnimatedPopup")
            : id_(id), is_open_(false), animation_progress_(0.0f),
              animation_type_(PopupAnimation::FadeIn),
              duration_(0.3f), // 300ms padrão
              easing_function_(Easing::EaseOutCubic)
        {
        }

        void Open()
        {
            is_open_ = true;
            is_closing_ = false;
            animation_start_ = std::chrono::steady_clock::now();
        }

        void Close()
        {
            is_closing_ = true;
            animation_start_ = std::chrono::steady_clock::now();
        }

        void SetAnimation(PopupAnimation type) { animation_type_ = type; }
        void SetDuration(float seconds) { duration_ = seconds; }
        void SetEasing(std::function<float(float)> easing) { easing_function_ = easing; }

        bool Begin(const char* name, ImVec2 size = ImVec2(0, 0))
        {
            if (!is_open_ && animation_progress_ <= 0.0f)
                return false;

            UpdateAnimation();

            // Salva estado original
            ImGuiStyle& style = ImGui::GetStyle();
            float original_alpha = style.Alpha;
            ImVec2 original_window_padding = style.WindowPadding;
            ImVec2 original_window_min_size = style.WindowMinSize;

            // Calcula posição central
            ImVec2 center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));

            if (size.x > 0 && size.y > 0)
                ImGui::SetNextWindowSize(size);

            // Aplica animação
            ApplyAnimation(style);

            // Flags do popup
            ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | 
                                   ImGuiWindowFlags_NoResize |
                                   ImGuiWindowFlags_NoMove |
                                   ImGuiWindowFlags_AlwaysAutoResize;

            bool result = ImGui::Begin(name, nullptr, flags);

            // Restaura estado original
            style.Alpha = original_alpha;
            style.WindowPadding = original_window_padding;
            style.WindowMinSize = original_window_min_size;

            return result;
        }

        void End()
        {
            ImGui::End();

            // Se a animação de fechamento terminou, fecha completamente
            if (is_closing_ && animation_progress_ <= 0.0f)
            {
                is_open_ = false;
                is_closing_ = false;
            }
        }

        bool IsOpen() const { return is_open_ || animation_progress_ > 0.0f; }
        float GetProgress() const { return animation_progress_; }

    private:
        void UpdateAnimation()
        {
            auto now = std::chrono::steady_clock::now();
            float elapsed = std::chrono::duration<float>(now - animation_start_).count();
            float raw_progress = std::min(elapsed / duration_, 1.0f);

            if (is_closing_)
            {
                animation_progress_ = 1.0f - raw_progress;
            }
            else
            {
                animation_progress_ = raw_progress;
            }

            // Aplica função de easing
            animation_progress_ = easing_function_(animation_progress_);
        }

        void ApplyAnimation(ImGuiStyle& style)
        {
            switch (animation_type_)
            {
                case PopupAnimation::FadeIn:
                    style.Alpha = animation_progress_;
                    break;

                case PopupAnimation::SlideDown:
                {
                    ImVec2 viewport_size = ImGui::GetMainViewport()->Size;
                    float offset = (1.0f - animation_progress_) * viewport_size.y * 0.5f;
                    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
                    ImGui::SetNextWindowPos(ImVec2(center.x, center.y - offset), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
                    style.Alpha = animation_progress_;
                    break;
                }

                case PopupAnimation::SlideUp:
                {
                    ImVec2 viewport_size = ImGui::GetMainViewport()->Size;
                    float offset = (1.0f - animation_progress_) * viewport_size.y * 0.5f;
                    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
                    ImGui::SetNextWindowPos(ImVec2(center.x, center.y + offset), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
                    style.Alpha = animation_progress_;
                    break;
                }

                case PopupAnimation::ScaleUp:
                {
                    float scale = animation_progress_;
                    style.WindowPadding = ImVec2(style.WindowPadding.x * scale, style.WindowPadding.y * scale);
                    style.WindowMinSize = ImVec2(style.WindowMinSize.x * scale, style.WindowMinSize.y * scale);
                    style.Alpha = animation_progress_;
                    break;
                }

                case PopupAnimation::Bounce:
                {
                    float bounce = Easing::EaseOutBounce(animation_progress_);
                    float scale = 0.8f + 0.2f * bounce;
                    style.WindowPadding = ImVec2(style.WindowPadding.x * scale, style.WindowPadding.y * scale);
                    style.Alpha = animation_progress_;
                    break;
                }

                case PopupAnimation::Elastic:
                {
                    float elastic = animation_progress_ < 1.0f ? Easing::EaseOutElastic(animation_progress_) : 1.0f;
                    float scale = 0.7f + 0.3f * elastic;
                    style.WindowPadding = ImVec2(style.WindowPadding.x * scale, style.WindowPadding.y * scale);
                    style.Alpha = std::min(animation_progress_ * 2.0f, 1.0f); // Fade rápido
                    break;
                }

                case PopupAnimation::Rotate:
                {
                    // ImGui não suporta rotação diretamente, então fazemos um efeito de "wobble"
                    float wobble = std::sin(animation_progress_ * IM_PI * 4) * (1.0f - animation_progress_) * 0.1f;
                    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
                    ImGui::SetNextWindowPos(ImVec2(center.x + wobble * 100, center.y), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
                    style.Alpha = animation_progress_;
                    break;
                }
            }
        }

    private:
        std::string id_;
        bool is_open_;
        bool is_closing_ = false;
        float animation_progress_;
        PopupAnimation animation_type_;
        float duration_;
        std::function<float(float)> easing_function_;
        std::chrono::steady_clock::time_point animation_start_;
    };

    // Popup de notificação toast
    class ToastNotification
    {
    public:
        enum class Type
        {
            Info,
            Success,
            Warning,
            Error
        };

        struct Toast
        {
            std::string message;
            Type type;
            float lifetime;
            float elapsed;
            float y_offset;
            bool is_closing;
        };

        void Show(const std::string& message, Type type = Type::Info, float lifetime = 3.0f)
        {
            Toast toast;
            toast.message = message;
            toast.type = type;
            toast.lifetime = lifetime;
            toast.elapsed = 0.0f;
            toast.y_offset = 0.0f;
            toast.is_closing = false;
            toasts_.push_back(toast);
        }

        void Update(float delta_time)
        {
            float target_y = 20.0f;

            for (auto it = toasts_.begin(); it != toasts_.end();)
            {
                it->elapsed += delta_time;

                // Animação de entrada (slide + fade)
                float show_progress = std::min(it->elapsed / 0.3f, 1.0f);
                
                // Começa a fechar antes do fim do lifetime
                if (it->elapsed > it->lifetime - 0.3f)
                {
                    it->is_closing = true;
                }

                // Animação de saída
                float hide_progress = 1.0f;
                if (it->is_closing)
                {
                    hide_progress = std::max(0.0f, (it->lifetime - it->elapsed) / 0.3f);
                }

                // Atualiza posição Y com smooth
                it->y_offset += (target_y - it->y_offset) * 10.0f * delta_time;
                target_y += 60.0f; // Espaço entre toasts

                if (it->elapsed >= it->lifetime)
                {
                    it = toasts_.erase(it);
                }
                else
                {
                    ++it;
                }
            }
        }

        void Render()
        {
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImVec2 work_pos = viewport->WorkPos;
            ImVec2 work_size = viewport->WorkSize;

            for (auto& toast : toasts_)
            {
                // Calcula alpha
                float alpha = 1.0f;
                if (toast.elapsed < 0.3f)
                    alpha = toast.elapsed / 0.3f;
                else if (toast.is_closing)
                    alpha = std::max(0.0f, (toast.lifetime - toast.elapsed) / 0.3f);

                // Posição
                float x_offset = toast.is_closing ? (1.0f - alpha) * 200.0f : (1.0f - alpha) * -200.0f;
                ImVec2 pos = ImVec2(work_pos.x + work_size.x - 320.0f + x_offset, 
                                   work_pos.y + toast.y_offset);

                ImGui::SetNextWindowPos(pos);
                ImGui::SetNextWindowSize(ImVec2(300, 0));

                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);
                ImGui::PushStyleColor(ImGuiCol_WindowBg, GetToastColor(toast.type));

                ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | 
                                       ImGuiWindowFlags_AlwaysAutoResize | 
                                       ImGuiWindowFlags_NoSavedSettings | 
                                       ImGuiWindowFlags_NoFocusOnAppearing | 
                                       ImGuiWindowFlags_NoNav | 
                                       ImGuiWindowFlags_NoMove;

                ImGui::Begin(("##Toast" + std::to_string((size_t)&toast)).c_str(), nullptr, flags);
                
                ImGui::Text("%s %s", GetToastIcon(toast.type), toast.message.c_str());
                
                ImGui::End();
                ImGui::PopStyleColor();
                ImGui::PopStyleVar(2);
            }
        }

    private:
        ImVec4 GetToastColor(Type type)
        {
            switch (type)
            {
                case Type::Success: return ImVec4(0.2f, 0.7f, 0.2f, 0.9f);
                case Type::Warning: return ImVec4(0.9f, 0.7f, 0.0f, 0.9f);
                case Type::Error: return ImVec4(0.9f, 0.2f, 0.2f, 0.9f);
                default: return ImVec4(0.2f, 0.4f, 0.8f, 0.9f);
            }
        }

        const char* GetToastIcon(Type type)
        {
            switch (type)
            {
                case Type::Success: return "✓";
                case Type::Warning: return "⚠";
                case Type::Error: return "✗";
                default: return "ℹ";
            }
        }

        std::vector<Toast> toasts_;
    };

    // Modal animado
    class AnimatedModal
    {
    public:
        AnimatedModal() : popup_("##AnimatedModal")
        {
            popup_.SetAnimation(PopupAnimation::Elastic);
            popup_.SetDuration(0.4f);
        }

        void Open(const std::string& title, const std::string& message)
        {
            title_ = title;
            message_ = message;
            result_ = -1;
            popup_.Open();
        }

        void Close()
        {
            popup_.Close();
        }

        // Retorna: -1 = ainda aberto, 0 = cancelado, 1 = confirmado
        int Draw()
        {
            if (!popup_.IsOpen())
                return result_;

            // Overlay escuro
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0.5f * popup_.GetProgress()));
            
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
            ImGui::Begin("##ModalOverlay", nullptr, 
                        ImGuiWindowFlags_NoDecoration | 
                        ImGuiWindowFlags_NoMove | 
                        ImGuiWindowFlags_NoSavedSettings);
            ImGui::End();
            
            ImGui::PopStyleColor();
            ImGui::PopStyleVar();

            // Modal
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20, 20));
            
            if (popup_.Begin(title_.c_str(), ImVec2(400, 0)))
            {
                ImGui::TextWrapped("%s", message_.c_str());
                
                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();

                float button_width = 120.0f;
                float spacing = ImGui::GetStyle().ItemSpacing.x;
                float total_width = button_width * 2 + spacing;
                float start_x = (ImGui::GetWindowWidth() - total_width) * 0.5f;

                ImGui::SetCursorPosX(start_x);
                
                if (ImGui::Button("Cancel", ImVec2(button_width, 0)))
                {
                    result_ = 0;
                    Close();
                }
                
                ImGui::SameLine();
                
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
                if (ImGui::Button("Confirm", ImVec2(button_width, 0)))
                {
                    result_ = 1;
                    Close();
                }
                ImGui::PopStyleColor();

                popup_.End();
            }
            
            ImGui::PopStyleVar(2);

            return result_;
        }

    private:
        AnimatedPopup popup_;
        std::string title_;
        std::string message_;
        int result_ = -1;
    };
}

// Exemplo de uso
class ExampleLayer
{
    ProEngine::AnimatedPopup popup_;
    ProEngine::ToastNotification toasts_;
    ProEngine::AnimatedModal modal_;
    float delta_time_ = 0.016f; // 60 FPS

    void OnImGuiRender()
    {
        // Janela principal
        if (ImGui::Begin("Animation Examples"))
        {
            if (ImGui::Button("Fade In Popup"))
            {
                popup_.SetAnimation(ProEngine::PopupAnimation::FadeIn);
                popup_.Open();
            }
            
            ImGui::SameLine();
            if (ImGui::Button("Slide Down"))
            {
                popup_.SetAnimation(ProEngine::PopupAnimation::SlideDown);
                popup_.Open();
            }
            
            ImGui::SameLine();
            if (ImGui::Button("Bounce"))
            {
                popup_.SetAnimation(ProEngine::PopupAnimation::Bounce);
                popup_.SetEasing(ProEngine::Easing::EaseOutBounce);
                popup_.Open();
            }
            
            ImGui::SameLine();
            if (ImGui::Button("Elastic"))
            {
                popup_.SetAnimation(ProEngine::PopupAnimation::Elastic);
                popup_.Open();
            }

            ImGui::Separator();

            if (ImGui::Button("Info Toast"))
                toasts_.Show("This is an info message!", ProEngine::ToastNotification::Type::Info);
            
            ImGui::SameLine();
            if (ImGui::Button("Success Toast"))
                toasts_.Show("Operation completed successfully!", ProEngine::ToastNotification::Type::Success);
            
            ImGui::SameLine();
            if (ImGui::Button("Error Toast"))
                toasts_.Show("An error occurred!", ProEngine::ToastNotification::Type::Error);

            ImGui::Separator();

            if (ImGui::Button("Open Modal"))
            {
                modal_.Open("Confirm Action", "Are you sure you want to proceed with this action?");
            }
        }
        ImGui::End();

        // Renderiza popup animado
        if (popup_.Begin("Animated Popup"))
        {
            ImGui::Text("This is an animated popup!");
            ImGui::Separator();
            ImGui::Text("Progress: %.2f", popup_.GetProgress());
            
            if (ImGui::Button("Close"))
            {
                popup_.Close();
            }
            
            popup_.End();
        }

        // Atualiza e renderiza toasts
        toasts_.Update(delta_time_);
        toasts_.Render();

        // Renderiza modal
        int modal_result = modal_.Draw();
        if (modal_result == 1)
        {
            toasts_.Show("Action confirmed!", ProEngine::ToastNotification::Type::Success);
        }
        else if (modal_result == 0)
        {
            toasts_.Show("Action cancelled", ProEngine::ToastNotification::Type::Info);
        }
    }
};