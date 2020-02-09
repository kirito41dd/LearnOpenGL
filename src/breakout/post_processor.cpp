#include <breakout/post_processor.h>
#include <iostream>

PostProcessor::PostProcessor(Shader shader, GLuint width, GLuint height)
    : PostProcessingShader(shader),
      Texture(),
      Width(width),
      Height(height)
{
    // 初始化帧缓冲 渲染缓冲
    glGenFramebuffers(1, &this->MSFBO);
    glGenFramebuffers(1, &this->FBO);
    glGenRenderbuffers(1, &this->RBO);

    // 使用多采样颜色缓冲区初始化渲染缓冲区存储（不需要深度/模板缓冲区）
    glBindFramebuffer(GL_FRAMEBUFFER, this->MSFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, this->RBO);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_RGB, width, height); // 为渲染缓冲对象分配存储
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, this->RBO); // 附加
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) // 检查
        std::cout << "ERROR::POSTPROCESSOR: Failed to initialize MSFBO" << std::endl;

    // 还要初始化FBO /纹理，以将多重采样的颜色缓冲区复制,用于着色器操作（用于后期处理效果）
    glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
    this->Texture.Generate(width, height, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->Texture.ID, 0); // 将纹理作为其颜色附件附加到framebuffer
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::POSTPROCESSOR: Failed to initialize FBO" << std::endl;

    // 换回默认
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Initialize render data and uniforms
    this->initRenderData();
    this->PostProcessingShader.use();
    this->PostProcessingShader.setInt("scene", 0);
    GLfloat offset = 1.0f / 300.0f;
    GLfloat offsets[9][2] = {
        { -offset,  offset  },  // top-left
        {  0.0f,    offset  },  // top-center
        {  offset,  offset  },  // top-right
        { -offset,  0.0f    },  // center-left
        {  0.0f,    0.0f    },  // center-center
        {  offset,  0.0f    },  // center - right
        { -offset, -offset  },  // bottom-left
        {  0.0f,   -offset  },  // bottom-center
        {  offset, -offset  }   // bottom-right
    };
    glUniform2fv(glGetUniformLocation(this->PostProcessingShader.ID, "offsets"), 9, (GLfloat*)offsets);
    GLint edge_kernel[9] = {
        -1, -1, -1,
        -1,  8, -1,
        -1, -1, -1
    };
    glUniform1iv(glGetUniformLocation(this->PostProcessingShader.ID, "edge_kernel"), 9, edge_kernel);
    GLfloat blur_kernel[9] = {
        1.0 / 16, 2.0 / 16, 1.0 / 16,
        2.0 / 16, 4.0 / 16, 2.0 / 16,
        1.0 / 16, 2.0 / 16, 1.0 / 16
    };
    glUniform1fv(glGetUniformLocation(this->PostProcessingShader.ID, "blur_kernel"), 9, blur_kernel);
}

void PostProcessor::BeginRender()
{
    glBindFramebuffer(GL_FRAMEBUFFER, this->MSFBO); // 向多重采样帧缓冲渲染
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void PostProcessor::EndRender()
{
    //现在将多重采样的颜色缓冲区通过中间FBO以存储到纹理
    glBindFramebuffer(GL_READ_FRAMEBUFFER, this->MSFBO); // 从 MSFBO 读
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->FBO); // 写入 FBO
    // 传送数据
    glBlitFramebuffer(0, 0, this->Width, this->Height, 0, 0, this->Width, this->Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    // 将读写都绑定回默认缓冲
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessor::Render(GLfloat time)
{
    // 把纹理渲染到屏幕
    this->PostProcessingShader.use();
    this->PostProcessingShader.setFloat("time", time);
    this->PostProcessingShader.setInt("confuse", this->Confuse);
    this->PostProcessingShader.setInt("chaos", this->Chaos);
    this->PostProcessingShader.setInt("shake", this->Shake);
    // 渲染纹理
    glActiveTexture(GL_TEXTURE0);
    this->Texture.Bind();
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

}

void PostProcessor::initRenderData()
{
    // 配置顶点数据
    GLuint VBO;
    GLfloat vertices[] = {
        // Pos        // Tex
        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f, 1.0f,

        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f
    };
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (GLvoid*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    // TODO: VBO释放了吗
}
