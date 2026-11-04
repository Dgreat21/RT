**(Work in progress, don't be confused of 'Dota 6' title)**

Github can't upload screenshots properly, so you can see them [here](https://github.com/rearming/RT/tree/master/screenshots)

## Сборка и запуск на macOS (Apple Silicon M1–M4)

Сборка нативная (arm64), Rosetta не нужна.

1. Инструменты командной строки Xcode и зависимости из Homebrew:
   ```bash
   xcode-select --install
   brew install sdl2 sdl2_ttf cmake git-lfs
   ```
   (`sdl2` в Homebrew — это `sdl2-compat`, слой SDL2 API поверх SDL3; OpenCL берётся из системного `OpenCL.framework`.)
2. Клонировать вместе с подмодулями (libft, jansson, textures) и LFS-файлами:
   ```bash
   git clone --recursive https://github.com/Dgreat21/RT.git && cd RT && git lfs pull
   ```
3. Собрать: `make` (если более новый компилятор выдаст новое предупреждение — `make WERROR=`).
4. Запускать **из корня репозитория** (пути к ядрам и ассетам относительные):
   ```bash
   ./RT scenes/cornell_box.json
   ```

Первый запуск сцены компилирует OpenCL-ядра (может занять десятки секунд), бинарники кешируются в `oclbins/`.
Ключ кеша — только опции компиляции, поэтому после правки `.cl`-файлов удаляйте `oclbins/`.

Альтернатива через CMake: `./compile.sh && cmake -S . -B build && cmake --build build` (нужен ещё `brew install pkgconf`).

Фреймворки в `lib/SDL2_libs` собраны только под x86_64 и на Apple Silicon не используются
(Makefile берёт их лишь как запасной вариант на Intel Mac без Homebrew SDL2).


![](/screenshots/qADw7VMNr44.jpg)

![](https://trello-attachments.s3.amazonaws.com/5e30819c83dc7b450f840e53/5e9be670859cd08b3974c0bb/10796989368d9572fc944ff38730651f/rsz_screen_shot_2020-02-25_at_200442-min.png)

![](/screenshots/Screenshot_from_2020-02-22_16-46-23.png)

![](https://trello-attachments.s3.amazonaws.com/5e30819c83dc7b450f840e53/5e9be670859cd08b3974c0bb/45828f1eae80f440a2c173b71153af53/rsz_screen_shot_2020-02-25_at_210820-min.png)

![](/screenshots/Screen_Shot_2020-03-17_at_13_16_31-min.png)

![](https://trello-attachments.s3.amazonaws.com/5e30819c83dc7b450f840e53/5e9be670859cd08b3974c0bb/ce18d5438cf728d09db06abd208438b9/rsz_1screen_shot_2020-01-29_at_131850-min.png)
