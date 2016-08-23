{ stdenv, emscripten }:

stdenv.mkDerivation rec {
  name = "cs4488_project01-${version}";
  version = "0.1";

  buildInputs = [ emscripten ];
}
