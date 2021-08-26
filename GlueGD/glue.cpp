/* GlueGD - A library that manages Geometry Dash mods
 * Copyright (C) 2021 Aly Cerruti
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>. */
#include "glue.hpp"

Glue::Glue() : lua() {
    lua.open_libraries(sol::lib::base);
}

void Glue::loadModule(std::filesystem::path module) {
    sol::protected_function_result moduleResult = lua.do_file(module.string());
    if(moduleResult.valid()) {
        // TODO store the modules somewhere
        return;
    } else {
        sol::error err = moduleResult;
        printf("Lua error in %s: %s\n", module.string().c_str(), err.what());
    }
}
