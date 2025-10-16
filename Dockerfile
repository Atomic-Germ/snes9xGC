# Snes9x GC/Wii Docker Build Environment

FROM devkitpro/devkitppc:20240612

# Install additional dependencies if needed
RUN rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /workspace

# Copy source code
COPY . /workspace/

# Default command
CMD ["bash"]