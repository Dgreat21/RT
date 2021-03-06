cmake_minimum_required(VERSION 3.13.1)
project(RT C)

set(CMAKE_C_STANDARD 11)

add_compile_options(-Wall -Wextra)

if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    add_compile_options(-O2)
endif()

# <----RT compile---->

set(debug_sources
        sources/debug/rt_debug_print_utils.c
        sources/debug/runtime_debug/rt_debug_info.c)

set(math_utils_sources
        sources/math_utils/rt_math_utils.c
        sources/math_utils/rt_rotation_utils.c
        sources/math_utils/rt_cl_float3_utils.c
        sources/math_utils/rt_vector_math.c
        sources/math_utils/rt_cl_float3_utils2.c)

set(OpenCL_sources
        sources/OpenCL/rt_opencl_init.c
        sources/OpenCL/rt_opencl_files_parser.c
        sources/OpenCL/rt_opencl_render.c
        sources/OpenCL/rt_opencl_memory_management.c
        sources/OpenCL/rt_opencl_utils.c
        sources/OpenCL/rt_opencl_prepare_memory.c
        sources/OpenCL/rt_init_render_kernel.c
        sources/OpenCL/rt_opencl_kernels_compile.c
        sources/OpenCL/rt_opencl_kernels_exec.c
        sources/OpenCL/rt_opencl_kernels_exec_utils.c
        sources/OpenCL/rt_opencl_prepare_mem_array.c
        sources/OpenCL/rt_opencl_cache_program.c)

set(parsing_sources
        sources/rt_parsing/rt_parse_scene.c
        sources/rt_parsing/json_parser/rt_add_to_scene.c
        sources/rt_parsing/json_parser/rt_json_utils.c
        sources/rt_parsing/json_parser/rt_json_utils2.c
        sources/rt_parsing/json_parser/rt_json_utils3.c
        sources/rt_parsing/json_parser/rt_parse_array.c
        sources/rt_parsing/json_parser/rt_parse_boolean.c
        sources/rt_parsing/json_parser/rt_parse_object.c
        sources/rt_parsing/json_parser/rt_parse_string.c
        sources/rt_parsing/json_parser/rt_parse_string2.c
        sources/rt_parsing/json_parser/rt_parse_textures.c
        sources/rt_parsing/json_parser/rt_parse_variable.c
        sources/rt_parsing/json_parser/rt_type_of_parametr.c
        )

set(event_handling_sources
        sources/rt_event_handling/rt_event_utils.c
        sources/rt_event_handling/rt_settings_change.c
        sources/rt_event_handling/rt_settings_info.c
        sources/rt_event_handling/rt_key_event_handing.c
        sources/rt_event_handling/rt_mouse_event_handling.c
        sources/rt_event_handling/rt_camera_moves.c
        sources/rt_event_handling/rt_event_handling.c
        sources/rt_event_handling/rt_key_event_handing2.c
        )

set(utils_sources
        sources/utils/rt_safe_malloc.c
        sources/utils/rt_raise_error.c
        sources/utils/rt_exit_clean.c
        sources/utils/rt_color_utils.c
        sources/utils/rt_hash.c
        sources/utils/rt_common_read_file.c)

set(SDL_utils_sources
        sources/SDL_utils/sdl_init.c
        )

set(obj_loading_sources
        sources/obj_loading/rt_correct_materials.c
        sources/obj_loading/rt_load_obj_files.c
        sources/obj_loading/rt_load_obj_utils.c
        sources/obj_loading/rt_load_obj_meshes.c
        sources/obj_loading/rt_load_obj_materials.c
        sources/obj_loading/rt_load_vertices.c)

set(init_sources
        sources/init/rt_init.c
        sources/init/rt_init_render_params.c
        sources/init/rt_load_textures.c
        sources/init/rt_load_textures_utils.c
        sources/init/rt_parse_options.c)

set(render_sources
        sources/render/rt_loop.c
        sources/render/render.c
        sources/render/rt_update_params.c
        )

set(kd_tree_sources
        sources/kd_tree/rt_kd_get_aabb.c
        sources/kd_tree/rt_kd_tree_build.c
        sources/kd_tree/rt_kd_tree_to_array.c
        sources/kd_tree/rt_kd_tree_utils.c
        sources/kd_tree/rt_kd_tree_get.c
        sources/kd_tree/rt_kd_pack_object_indices.c
        sources/kd_tree/rt_kd_tree_export.c
        sources/kd_tree/rt_kd_tree_import.c
        sources/kd_tree/rt_kd_tree_build_aabb_utils.c
        sources/kd_tree/rt_kd_build_recursive.c
        sources/kd_tree/rt_kd_split_buckets_sah.c)

set(gui_sources
        sources/gui/rt_gui_init.c
        sources/gui/rt_handle_event_gui.c
        sources/gui/gui_utils/rt_gui_color_utils.c
        sources/gui/buttons/rt_gui_buttons_render.c
        sources/gui/buttons/rt_gui_button_callback.c
        sources/gui/buttons/rt_buttons_border.c
        sources/gui/buttons/rt_button_label.c
        sources/gui/buttons/utils/rt_gui_change_algo.c
        sources/gui/buttons/rt_gui_init_buttons.c
        sources/gui/buttons/utils/rt_check_hover.c
        sources/gui/buttons/utils/rt_check_click.c
        sources/gui/buttons/utils/create_screenshot.c
        sources/gui/buttons/utils/name_generator.c
        sources/gui/buttons/utils/get_window_data.c
        sources/gui/text_box/render_text_box.c
        sources/gui/text_box/init_text_box.c
        sources/gui/text_box/rt_gui_text_box_init.c
        sources/gui/buttons/utils/check_positions.c
        sources/gui/buttons/rt_gui_buttons_render2.c sources/gui/fuck_norminette.c sources/gui/rt_sync_options.c)


set(sources
        ${debug_sources}
        ${math_utils_sources}
        ${OpenCL_sources}
        ${parsing_sources}
        ${event_handling_sources}
        ${utils_sources}
        ${SDL_utils_sources}
        ${obj_loading_sources}
        ${init_sources}
        ${render_sources}
        ${kd_tree_sources}
        ${gui_sources}
        ${image_save_sources}
        sources/gui/fuck_norminette.c)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake_search_modules)

    find_package(SDL2 REQUIRED)
    find_package(SDL2_ttf REQUIRED)
    find_package(OpenCL REQUIRED)

    set(graphic_libs_include_dirs
            ${SDL2_INCLUDE_DIRS}
            ${SDL2_TTF_INCLUDE_DIRS}
            ${OpenCL_INCLUDE_DIRS})

    set(graphic_libs_link_names
            SDL2
            SDL2_ttf
            OpenCL)

else()
    set(graphic_libs_include_dirs
            lib/SDL2_libs/SDL2.framework/Headers
            lib/SDL2_libs/SDL2_ttf.framework/Headers)
    set(graphic_libs_dirs
            lib/SDL2_libs/)
    set(graphic_libs_link_names
            "-F ${PROJECT_SOURCE_DIR}/lib/SDL2_libs -framework SDL2 -framework SDL2_ttf"
            "-framework OpenCL")
endif()

message("graphic_libs_include_dirs: " ${graphic_libs_include_dirs})
message("graphic_libs_dirs: " ${graphic_libs_dirs})
message("graphic_libs_link_names: " ${graphic_libs_link_names})

include_directories(
        libft/includes
        includes
        includes/external
        ${graphic_libs_include_dirs}
)

link_directories(
        libft
        libft/ft_printf
        ${graphic_libs_dirs}
        lib/jansson/build/lib/
)

message(${sources})

add_executable(RT
        sources/main.c
        ${sources}
)

target_link_libraries(RT
        "ftprintf"
        "ft"
        ${graphic_libs_link_names}
        "m"
        "jansson"
        )