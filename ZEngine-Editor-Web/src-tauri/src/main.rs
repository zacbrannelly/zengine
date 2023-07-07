// Prevents additional console window on Windows in release, DO NOT REMOVE!!
#![cfg_attr(not(debug_assertions), windows_subsystem = "windows")]

#[cfg(target_os = "macos")]
#[macro_use]
extern crate objc;
use tauri::Manager;

extern crate dlopen;
#[macro_use]
extern crate dlopen_derive;
use dlopen::wrapper::{Container, WrapperApi};

extern crate cty;

extern crate base64;

use serde::{Serialize, Deserialize};

#[repr(C)]
pub struct FrameInfo     {
    pub width: cty::c_int,
    pub height: cty::c_int,
    pub pitch: cty::c_int,
    pub size: cty::c_int,
    pub data: *mut cty::uint8_t,
}

#[derive(Serialize, Deserialize)]
pub struct FrameInfoSerde {
    pub width: i32,
    pub height: i32,
    pub pitch: i32,
    pub size: i32,
    pub dataBase64: String,
}

#[derive(WrapperApi)]
struct Api<> {
    ZEngine_Editor_Web_SetupEngine: unsafe extern "C" fn(window_ptr: i32),
    ZEngine_Editor_Web_RenderFrame: unsafe extern "C" fn(frame_info: *mut FrameInfo),
    ZEngine_Editor_Web_ShutdownEngine: unsafe extern "C" fn(),
}

// Learn more about Tauri commands at https://tauri.app/v1/guides/features/command
#[tauri::command]
fn greet(name: &str) -> String {
    format!("Hello, {}! You've been greeted from Rust! Wonder what would've happened if I changed this", name)
}

#[tauri::command]
fn render(cont: tauri::State<Container<Api>>) -> Result<String, String> {
    unsafe {
        let mut frame_info = FrameInfo {
            width: 0,
            height: 0,
            pitch: 0,
            size: 0,
            data: std::ptr::null_mut(),
        };
        cont.ZEngine_Editor_Web_RenderFrame(&mut frame_info);
        println!("FrameInfo: width: {}, height: {}, pitch: {}, size: {}, data: {:p}", frame_info.width, frame_info.height, frame_info.pitch, frame_info.size, frame_info.data);

        let frame_info_serde = FrameInfoSerde {
            width: frame_info.width,
            height: frame_info.height,
            pitch: frame_info.pitch,
            size: frame_info.size,
            // data: Vec::from_raw_parts(frame_info.data, frame_info.size as usize, frame_info.size as usize),
            // data: Vec::<u8>::new(),
            dataBase64: base64::encode(std::slice::from_raw_parts(frame_info.data, frame_info.size as usize)),
        };

        Ok(frame_info_serde.dataBase64)
    }
}

fn main() {
    let app = tauri::Builder::default()
        .manage(unsafe { 
            Container::load("libZEngine-Editor-Web.dylib") as Result<Container<Api>, _>
        }.expect("Could not open library or load symbols"))
        .setup(|app| {
            let main_window = app.get_window("main").unwrap();

            #[cfg(target_os = "macos")]
            unsafe {
                let window_ptr = main_window.ns_window().ok().unwrap();

                let cont = app.state::<Container<Api>>();
                cont.ZEngine_Editor_Web_SetupEngine(window_ptr as i32);

                let mut frame_info = FrameInfo {
                    width: 0,
                    height: 0,
                    pitch: 0,
                    size: 0,
                    data: std::ptr::null_mut(),
                };
                cont.ZEngine_Editor_Web_RenderFrame(&mut frame_info);

                println!("FrameInfo: width: {}, height: {}, pitch: {}, size: {}, data: {:p}", frame_info.width, frame_info.height, frame_info.pitch, frame_info.size, frame_info.data);
            }

            Ok(())
        })
        .invoke_handler(tauri::generate_handler![greet, render])
        .build(tauri::generate_context!())
        .expect("error while building tauri application");

    app.run(|app_handle, event| match event {
            tauri::RunEvent::ExitRequested { api, .. } => {
                unsafe {
                    let cont = app_handle.state::<Container<Api>>();
                    cont.ZEngine_Editor_Web_ShutdownEngine();
                }
            }
            _ => {}
        });
}
