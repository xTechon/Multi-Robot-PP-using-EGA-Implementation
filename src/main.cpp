#include "main.h"

#include "fileHandler.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"
#include "interact.h"

// Example code from:
// https://github.com/ocornut/imgui/blob/master/examples/example_glfw_opengl3/main.cpp

static void glfw_error_callback(int error, const char* description) {
  fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int, char**) {
  // Setup window
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit()) return 1;

  const char* glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

  // Create window
  GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGUI GLFW+OpenGL3 example", NULL, NULL);
  if (window == NULL) return 1;
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1); // VSync Enable

  // Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImPlot::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void) io;

  // Dear ImGui style
  ImGui::StyleColorsDark();

  // Setup Platform/Render backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  // Window booleans
  bool show_demo_window    = true;
  bool show_another_window = false;
  ImVec4 clear_color       = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  // filePath variables
  std::string* filePath = nullptr;

  // file loader variables
  FileHandler* filePicker = new FileHandler();
  bool fileLoad           = false;
  Grid* terrain           = (Grid*) nullptr;
  Interact* plots         = new Interact();

  // Main window loop
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    // start Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // windows here
    // simple window
    {
      static float f     = 0.0f;
      static int counter = 0;
      ImGui::Begin("Hello world!");
      ImGui::Text("Some useful text.");
      ImGui::Checkbox("Another Window", &show_another_window);
      ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
      ImGui::ColorEdit3("clear color", (float*) &clear_color);
      filePath = filePicker->drawGUI(filePath, &fileLoad);
      if (filePath != nullptr) {
        const char* file = filePath->c_str();
        ImGui::Text("File Choosen:\n%s", file);
        // std::cout << fmt::format("{}", *fileP) << std::endl;
        if (fileLoad == false) {
          // load the terrain
          terrain = filePicker->importGrid(filePath, terrain);
          ImGui::Text("Terrain Loaded");
          // Breakpoint toggle
          if (ImGui::GetIO().KeyAlt) {
            printf(""); // Set a debugger breakpoint here!
          }
        }
      }
      if (ImGui::GetIO().KeyAlt) {
        printf(""); // Set a debugger breakpoint here!
      }

      ImGui::End();
    }

    if (show_another_window) {
      ImGui::Begin("Another Window", &show_another_window);
      ImGui::Text("Hi Another Window, I'm Dad!");
      if (ImGui::Button("Close Me")) show_another_window = false;
      ImGui::End();
    }

    plots->drawMapTest();

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
  }

  // cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  ImPlot::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
