# Mini-CAD

![Zrzut ekranu z 2024-11-07 13-35-54](https://github.com/user-attachments/assets/dcc3e459-2a1f-448e-967b-541dbafe58b7)

## About Project

A Computer-aided design software for modelling smooth 3D surfaces

**Features:**
- 3D manipulation tools,
- Creating Bezier, B-Spline and interpolated curves,
- Designing models using Bezier patches and Gregory patches,
- Calculating intersections between surfaces,
- Exporting and Importing created models.

## Navigation

<table>
    <tbody>
		<tr>
            <th colspan=2> General</th>
        </tr>
		<tr>
            <td><kbd>LMB</kbd></td>
            <td>Menu navigation</td>
        </tr>
		<tr>
            <td><kbd>MMB</kbd></td>
            <td>Cursor Position Selection</td>
        </tr>
		<tr>
            <td>Double <kbd>LMB</kbd></td>
            <td>Clear current selection</td>
        </tr>
		<tr>
            <th colspan=2> Camera</th>
        </tr>
		<tr>
            <td><kbd>RMB</kbd></td>
            <td>Camera rotation</td>
        </tr>
		<tr>
            <td><kbd>Scroll</kbd></td>
            <td>Camera zoom</td>
        </tr>
		<tr>
            <td><kbd>WASD</kbd></td>
            <td>Camera movement along the XY plane</td>
        </tr>
	</tbody>
</table>

## Compilation

### Windows

Program is built using using CMake build system and Ninja generator, and was tested using MinGW 11.0 win64.

```
git clone --recurse-submodules -j8 https://github.com/faliszewskii/mini-cad
cd mini-cad
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_MAKE_PROGRAM="path/to/ninja.exe"
cmake --build build --config Release
```

App can be executed from Windows Explorer or through Windows Powershell by:

```
.\build\src\mini-cad.exe
```
## Gallery

### Example model (A flying fish of the family Exocoetidae)
![image](https://github.com/user-attachments/assets/d3730196-6a3d-40a4-b4bc-d7ef3608a84f)

### Real-life model milled using generated paths
![Rybka](https://github.com/user-attachments/assets/a153136d-9155-4c30-a68e-dee23b391d3b)

### Precise surface manipulation and connections
![image](https://github.com/user-attachments/assets/65e7decd-a035-49ec-ad86-6a0f3ed4364d)

### Detecting surface intersections
![image](https://github.com/user-attachments/assets/06a9d6b2-990e-4c51-b3dc-75a27ef80d12)


## Used Libraries

| Library | Licence |
| :---:   | :---:   |
| [GLFW](https://github.com/glfw/glfw) | [Zlib](https://github.com/glfw/glfw?tab=Zlib-1-ov-file#readme) |
| [GLM](https://github.com/g-truc/glm) | [Modified MIT](https://github.com/g-truc/glm?tab=License-1-ov-file#readme)|
| [ImGui](https://github.com/ocornut/imgui) | [MIT](https://github.com/ocornut/imgui?tab=MIT-1-ov-file#readme)| 
| [GLAD](https://github.com/Dav1dde/glad) | [MIT](https://github.com/Dav1dde/glad?tab=License-1-ov-file#readme) | 
| [Scene-Serializer](https://github.com/bjadczak/Scene-Serializer/tree/405a68e9d7e8c9a24665562c9e9fa1c42878d8e2) | [Apache License 2.0](https://github.com/bjadczak/Scene-Serializer/blob/main/LICENSE) | 
| [nativefiledialog-extended](https://github.com/btzy/nativefiledialog-extended/tree/2a7440a3a52feba815effbfcadc280de63eb2acb) | [ZLib](https://github.com/btzy/nativefiledialog-extended/blob/master/LICENSE) | 
| [uuid](https://github.com/uuidjs/uuid) | [MIT](https://github.com/uuidjs/uuid?tab=MIT-1-ov-file#readme) | 
| [ImGuizmo](https://github.com/CedricGuillemet/ImGuizmo) | [MIT](https://github.com/CedricGuillemet/ImGuizmo?tab=MIT-1-ov-file#readme) | 

