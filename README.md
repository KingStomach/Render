# Render
使用C++实现的模仿Opengl工作原理的软光栅化渲染器，项目中使用的模型为[tinyrender](https://github.com/ssloy/tinyrenderer) 里面的开源模型。

## 实现功能
- 一套完整的向量、矩阵运算库
- 一套完整的解析Bitmap库，用于读取纹理、纹理采样、输出渲染等
- 一套解析obj模型文件库
- 背面剔除， zbuffer深度缓存
- Gouraud着色频率，Phong光照模型
- 使用C++编写的顶点着色器（Vertex Shader）与片元着色器(Fragment Shader)
- 三角重心坐标插值
- 双线性插值纹理查询