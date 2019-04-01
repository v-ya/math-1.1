#ifndef	_package_vya_gspace_func_h_
#define	_package_vya_gspace_func_h_

// program
func(createProgram);
func(deleteProgram);
func(attachShader);
func(linkProgram);
func(useProgram);

// buffer
func(createBuffer);
func(deleteBuffer);
func(bufferSetData);
func(bufferSubData);
func(useBuffer);

// vertexAttrib
func(createVertexAttrib);
func(deleteVertexAttrib);
func(finalVertexAttrib);
func(vertexAttribPointer);
func(enableVertexAttrib);
func(disableVertexAttrib);
func(useVertexAttrib);

// model
func(createModel);
func(deleteModel);
func(finalModel);
func(mcRunScript);
func(mcSyncUniform);
func(mcBindBuffer);
func(mcBindVertexAttrib);
func(mcDrawArrays);
func(mcDrawElements);

func(modelDraw);

// draw
func(drawClear);
func(drawFlush);
func(drawFinish);
func(drawArrays);
func(drawElements);

// system
func(initWindow);

// test
func(test);

#endif

