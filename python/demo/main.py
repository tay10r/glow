import uikitpy.libs.glfw as glfw
import uikitpy.libs.gl as gl
import uikitpy.libs.imgui as imgui
import uikitpy.libs.implot as implot
import uikitpy.libs.pfd as pfd
from uikitpy.libs.imgui import WindowFlags
import numpy as np

def main():
    if not glfw.init():
        print('Failed to initialize GLFW.')
        return

    window = glfw.Window(640, 480, 'Test', None)

    window.set_imgui_config_path('uikitpy_demo.ini')

    flag = False
    value = 0
    plot_data = np.random.default_rng().standard_normal(128)

    dialog = None

    while not window.should_close():

        glfw.poll_events()

        if dialog is not None and dialog.ready():
            result = dialog.result()
            dialog = None

        window.begin_frame()

        w, h = window.get_framebuffer_size()
        gl.viewport(0, 0, w, h)
        gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT)

        if imgui.begin_main_menu_bar():
            if imgui.begin_menu('File'):
                if imgui.menu_item('Exit'):
                    window.set_should_close(True)
                imgui.end_menu()
            imgui.end_menu_bar()

        if imgui.begin('Test window'):
            if imgui.button('Test'):
                print('Button was clicked.')
            changed, flag = imgui.checkbox('Flag', flag)
            changed, value = imgui.slider_int('Value', value, 0, 10)
        imgui.end()

        if imgui.begin('Plot window'):
            if implot.begin_plot('Example Plot'):
                implot.plot_line('Example Line Plot', plot_data)
                implot.end_plot()
            if implot.begin_plot('Example Image Plot'):
                implot.end_plot()
            if imgui.button('Browse'):
                dialog = pfd.OpenFileDialog('Open Image')

        imgui.end()


        window.end_frame()

    window.close()

    glfw.terminate()


main()
