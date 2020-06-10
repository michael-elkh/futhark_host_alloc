-- <FHA is a pinned memory allocation lib for [futhark](https://github.com/diku-dk/futhark).>
--     Copyright (C) 2020 El Kharroubi Michaël

--     This program is free software: you can redistribute it and/or modify
--     it under the terms of the GNU General Public License as published by
--     the Free Software Foundation, either version 3 of the License, or
--     (at your option) any later version.

--     This program is distributed in the hope that it will be useful,
--     but WITHOUT ANY WARRANTY; without even the implied warranty of
--     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--     GNU General Public License for more details.

--     You should have received a copy of the GNU General Public License
--     along with this program.  If not, see <https://www.gnu.org/licenses/>.
let identity (input: i32) : i32 = input
-- Does nothing, it's only useful to avoid optimization.
entry main [n] (tab: [n]i32) : [n]i32 = map identity tab