# 🎥 Inserción de Dos Videos en Plantilla mediante Polígonos con OpenCV

![Static Badge](https://img.shields.io/badge/C%2B%2B-17-blue?style=plastic&logo=cplusplus&logoSize=auto&label=C%2B%2B&link=https%3A%2F%2Fes.wikipedia.org%2Fwiki%2FC%252B%252B)
[![C++](https://img.shields.io/badge/C%2B%2B-17-blue)](https://isocpp.org/)
[![OpenCV](https://img.shields.io/badge/OpenCV-4.10-green)](https://opencv.org/)
[![Visual Studio Code](https://img.shields.io/badge/IDE-VS%20Code-blue)](https://code.visualstudio.com/)
[![License: MIT](https://img.shields.io/badge/License-MIT-purple)](LICENSE)

Este proyecto permite **insertar simultáneamente dos videos (o captura en vivo de cámara y un video local)** dentro de una imagen plantilla, en regiones definidas por el usuario mediante la selección de puntos que conforman polígonos. Es ideal para aplicaciones de **streaming, presentaciones o superposición creativa de video**.

Desarrollado en **C++** usando **Visual Studio Code** y librerías de **OpenCV (compiladas con CMake)**.

---

## ✨ Características
- 📌 Definición interactiva de regiones poligonales para insertar videos.  
- 📷 Soporte para cámara en vivo y archivos de video locales.  
- 🎨 Filtros en tiempo real:
  - Ecualización de histograma  
  - Ecualización adaptativa (CLAHE)  
  - Operaciones morfológicas (Apertura)  
  - Suavizado bilateral  
- 🔁 Reinicio automático del video al finalizar.  
- 🖱️ Interacción mediante clic izquierdo para definir puntos y teclado para activar/desactivar filtros.

---

## 🖥️ Tecnologías usadas
- **Lenguaje:** C++  
- **IDE:** Visual Studio Code  
- **Librerías:**  
  - OpenCV (compilado con CMake)  
  - OpenCV DNN (para compatibilidad futura con GPU)  
- **Sistema Operativo recomendado:** Windows 10/11  

---

## 📂 Estructura del proyecto
