/*
    This file is part of qtoyunda software
    Copyright (C) 2008  Sylvain "Skarsnik" Colinet <scolinet@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


template<typename T> void printToyundaText(T &stream, const ToyundaText &tt)
{
  stream << "Text : " << tt.text << "\n";
  stream << "Frame  : (" << tt.start << ", " << tt.stop << ")\n";
  stream << "Number of pipe : " << tt.pipeNumber << "\n";
  stream << "Position : (" << tt.posx << ", " << tt.posy << ")\n";
  stream << "Size : " << tt.size << "\n";
  stream << "Colors : -- " << tt.color1.red() << 
                    "," << tt.color1.green() <<
                    "," << tt.color1.blue() <<
                    "," << tt.color1.alpha() << "\n";
  if (tt.color2.isValid()) {
    stream << " -- " << tt.color2.red() << 
                    "," << tt.color2.green() <<
                    "," << tt.color2.blue() <<
                    "," << tt.color2.alpha();
    stream << " -- " << tt.fadingcolor.red() << 
                    "," << tt.fadingcolor.green() <<
                    "," << tt.fadingcolor.blue() <<
                    "," << tt.fadingcolor.alpha() << "\n";
  }
}

template<typename T> void  printToyundaSyl(T &stream, const ToyundaSyl &ts)
{
  stream << "Frame  : (" << ts.start << ", " << ts.stop << ")\n";
  stream << "Number of pipe : " << ts.pipeNumber << "\n";
  stream << "Position : " << ts.pos << "\n";
  if (ts.alpha != -1)
      stream << "Alpha : " << ts.alpha << "\n";
}
