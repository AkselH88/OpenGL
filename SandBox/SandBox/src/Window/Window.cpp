#include "Window.h"

Window::Window()
    :m_Window(nullptr), m_Gui(nullptr), vSync(true)
{
    if (!glfwInit())
        data.Initialized = false;
    else
    {
        setWindowHint();

        data.Title = "SandBox";
        data.Height = HEIGHT;
        data.Width = WIDTH;

        m_Window = glfwCreateWindow(data.Width, data.Height, data.Title.c_str(), NULL, NULL);
        if (!m_Window)
        {
            glfwTerminate();
            data.Initialized = false;
        }
        else
        {
            /* Make the window's context current */
            glfwMakeContextCurrent(m_Window);

            OnEventInit();

            SetVSync(vSync);

            // tell GLFW to capture our mouse
            glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

            if (glewInit() != GLEW_OK)
            {
                data.Initialized = false;
            }
            else
            {
                std::cout << glGetString(GL_VERSION) << std::endl;

                GLCall(glEnable(GL_BLEND));
                GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

                GLCall(glEnable(GL_DEPTH_TEST));
                GLCall(glEnable(GL_CULL_FACE));

                m_Gui = new Gui(m_Window);

                data.Initialized = true;
                data.IsOpen = !glfwWindowShouldClose(m_Window);

                renderer.Clear();
                glfwSwapBuffers(m_Window);

                renderer.Clear();
                glfwSwapBuffers(m_Window);
            }
        }
    }
}
Window::~Window()
{
    OnTerminate();
}

void Window::OnTerminate()
{
    delete m_Gui;
    m_Gui = nullptr;

    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

void Window::setWindowHint()
{
    int major = 0;
    int minor = 0;
    int rev = 0;
    glfwGetVersion(&major, &minor, &rev);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

bool Window::IsOpen()
{
    return data.IsOpen;
}

void Window::SetVSync(bool VSync)
{
    glfwSwapInterval(VSync);
}

void Window::OnEventInit()
{
    glfwSetWindowUserPointer(m_Window, (void*)&data);

    /* On Window Re Size Event */
    glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
    {
        WindowContentConteiner& OnEvent = *(WindowContentConteiner*)glfwGetWindowUserPointer(window);
        OnEvent.Height = height;
        OnEvent.Width = width;
        OnEvent.camera.setWidthAndHeight((float)width, (float)height);

        glViewport(0, 0, width, height);
    });

    /* On Keyboard Event */
    glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        WindowContentConteiner& OnEvent = *(WindowContentConteiner*)glfwGetWindowUserPointer(window);

        switch (action)
        {
            case GLFW_PRESS:
            {
                if (key == GLFW_KEY_ESCAPE)
                {
                    OnEvent.IsOpen = false;
                    break;
                }

                OnEvent.processKeyOnPress(key);
                break;
            }
            case GLFW_REPEAT:
            {
                break;
            }
            case GLFW_RELEASE:
            {
                OnEvent.processKeyOnRelease(key);
                break;
            }
        }
    });

    /* On Mouse Scroll Event */
    glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset)
    {
        WindowContentConteiner& OnEvent = *(WindowContentConteiner*)glfwGetWindowUserPointer(window);
        OnEvent.camera.ProcessMouseScroll((float)yoffset);
    });

    /* On Mouse Button Event */
    glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
    {
        WindowContentConteiner& OnEvent = *(WindowContentConteiner*)glfwGetWindowUserPointer(window);

        switch (action)
        {
            case GLFW_PRESS:
            {
                OnEvent.mouse.firstClick = true;
                switch (button)
                {
                    case GLFW_MOUSE_BUTTON_1: // Left button
                    {
                        OnEvent.mouse.Left = true;
                        break;
                    }
                    case GLFW_MOUSE_BUTTON_2: // Right button
                    {
                        OnEvent.mouse.Right = true;
                        break;
                    }
                }
                break;
            }
            case GLFW_RELEASE:
            {
                switch (button)
                {
                    case GLFW_MOUSE_BUTTON_1: // Left button
                    {
                        OnEvent.mouse.Left = false;
                        break;
                    }
                    case GLFW_MOUSE_BUTTON_2: // Right button
                    {
                        OnEvent.mouse.Right = false;
                        break;
                    }
                }
                break;
            }
        }
    });

    /* On Mouse Cursor Move Event */
    glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos)
    {
        WindowContentConteiner& OnEvent = *(WindowContentConteiner*)glfwGetWindowUserPointer(window);
        OnEvent.processMouseOnMove((float)xpos, (float)ypos);
    });

    glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
    {
        WindowContentConteiner& OnEvent = *(WindowContentConteiner*)glfwGetWindowUserPointer(window);
        OnEvent.IsOpen = false;
    });
}

void Window::updateTime()
{
    float currentFrame = (float)glfwGetTime();
    data.deltaTime = currentFrame - data.lastFrame;
    data.lastFrame = currentFrame;
}

void Window::OnUpdate()
{
    renderer.Clear();
    updateTime();
    m_Gui->OnUpdate();

    ImGui::Checkbox("VSync", &vSync);
    SetVSync(vSync);

    data.camera.OnUpdate(data.deltaTime, &data.buttons);
}

/* Swap Buffers */
void Window::OnRender()
{
    m_Gui->OnRender();

    glfwSwapBuffers(m_Window);
    glfwPollEvents();
}

void Window::WindowContentConteiner::processKeyOnPress(int key)
{
    switch (key)
    {
        case GLFW_KEY_W:
        {
            buttons.W = true;
            break;
        }
        case GLFW_KEY_S:
        {
            buttons.S = true;
            break;
        } 
        case GLFW_KEY_A:
        {
            buttons.A = true;
            break;
        }
        case GLFW_KEY_D:
        {
            buttons.D = true;
            break;
        }
        case GLFW_KEY_SPACE:
        {
            buttons.SPACE = true;
            break;
        }
        case GLFW_KEY_LEFT_SHIFT:
        {
            buttons.SHIFT = true;
            break;
        }
        default:
            break;
    }
}

void Window::WindowContentConteiner::processKeyOnRelease(int key)
{
    switch (key)
    {
        case GLFW_KEY_W:
        {
            buttons.W = false;
            camera.setCurrentAnimation(SET_ANIMATION::IDLE);
            break;
        }
        case GLFW_KEY_S:
        {
            buttons.S = false;
            break;
        }
        case GLFW_KEY_A:
        {
            buttons.A = false;
            break;
        }
        case GLFW_KEY_D:
        {
            buttons.D = false;
            break;
        }
        case GLFW_KEY_SPACE:
        {
            buttons.SPACE = false;
            break;
        }
        case GLFW_KEY_LEFT_SHIFT:
        {
            buttons.SHIFT = false;
            break;
        }
    }
}

void Window::WindowContentConteiner::processMouseOnMove(float xpos, float ypos)
{
    if(mouse.Left || mouse.Right)
    {
        if (mouse.firstClick)
        {
            mouse.lastX = xpos;
            mouse.lastY = ypos;
            mouse.firstClick = false;
        }

        float xoffset = (xpos - mouse.lastX);
        float yoffset = (mouse.lastY - ypos);

        mouse.lastX = (float)xpos;
        mouse.lastY = (float)ypos;

        if((!mouse.Left && mouse.Right) || (mouse.Left && mouse.Right))
            camera.ProcessRightMouseMovement(xoffset, yoffset);
        else
            camera.ProcessLeftMouseMovement(xoffset, yoffset);
    }
}