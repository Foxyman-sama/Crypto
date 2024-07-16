#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_stdlib.h>

#include <memory>

#include "input.hpp"

class Alignment {
 public:
  static void center_by_width(float width) { ImGui::SetCursorPosX((ImGui::GetWindowSize().x - width) * 0.5f); }
};

class GLFWBackendWindow {
 public:
  void show(const char *title, int width, int height) {
    glfw_init();
    create_window(title, width, height);
    imgui_init();

    while (!glfwWindowShouldClose(window)) {
      handle_poll();
    }

    clear();
  }

 private:
  void glfw_init() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  }

  void create_window(const char *title, int width, int height) {
    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
  }

  void imgui_init() {
    ImGui::CreateContext();

    auto &context { ImGui::GetIO() };
    context.IniFilename = "";
    context.LogFilename = "";
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 150");
  }

  void handle_poll() {
    glfwPollEvents();
    create_frame();
    show_main_window();
    render();
  }

  void create_frame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
  }

  virtual void show_main_window() = 0;

  std::unique_ptr<char> get_input_text() {
    constexpr auto text_size { 100 };
    auto *text { new char[text_size] {} };
    constexpr auto elem_size { 200 };
    Alignment::center_by_width(elem_size);
    ImGui::SetNextItemWidth(elem_size);
    ImGui::InputText("###first", text, text_size);
    return std::unique_ptr<char> { text };
  }

  void render() {
    ImGui::Render();

    auto display_w { 0 };
    auto display_h { 0 };
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_h, display_h);
    glClearColor(1.00f, 1.00f, 1.00f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
  }

  void clear() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
  }

 protected:
  GLFWwindow *window;
};

class Window : public GLFWBackendWindow {
 public:
  Window(Input &input, DataView &data_view) : input { input }, data_view { data_view } {}

 private:
  void show_main_window() override {
    ImGui::SetNextWindowSize({ window_width, window_height });
    ImGui::Begin("Crypto", nullptr);

    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
    ImGui::Text("Text for encrypting:");
    ImGui::SameLine();
    get_text_for_encryption();

    ImGui::Spacing();
    ImGui::Text("Key for encrypting: ");
    ImGui::SameLine();
    get_key();

    ImGui::Spacing();
    ImGui::Text("Output:             ");
    ImGui::SameLine();
    show_output_text();
    ImGui::PopStyleColor();

    ImGui::Spacing();
    Alignment::center_by_width(button_elem_width);
    handle_buttons_clicks();

    ImGui::End();
  }

  void get_text_for_encryption() {
    ImGui::SetNextItemWidth(input_elem_width);

    std::string temp { text_for_encryption };
    if (ImGui::InputText("###input_1", &temp)) {
      this->text_for_encryption = temp;
    }
  }

  void get_key() {
    ImGui::SetNextItemWidth(input_elem_width);

    std::string temp { key };
    if (ImGui::InputText("###input_2", &temp)) {
      this->key = temp;
    }
  }

  void handle_buttons_clicks() {
    if (ImGui::Button(button_text, { button_elem_width, button_elem_height })) {
      input.encrypt("caesar", text_for_encryption, key.c_str());
      std::cout << data_view.output_text << '\n';
    }
  }

  void show_output_text() {
    ImGui::SetNextItemWidth(input_elem_width);
    ImGui::InputText("###output", &data_view.output_text);
  }

 private:
  static constexpr auto text_input_length { 100 };
  static constexpr auto input_elem_width { 200 };

  static constexpr auto output_elem_width { 200 };

  static constexpr auto button_text { "Execute" };
  static constexpr auto button_elem_width { 200 };
  static constexpr auto button_elem_height { 20 };

  static constexpr auto window_width { 375 };
  static constexpr auto window_height { 150 };

  Input &input;
  DataView &data_view;

  std::string text_for_encryption;
  std::string key;
};

#endif