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

// See https://twitter.com/__phantomderp/status/1431322478442422278
template<typename Handler>
bool sol_lua_check(
    sol::types<Module>, lua_State* L,
    int index, Handler&& handler,
    sol::stack::record& tracking
) {
    int absolute_index = lua_absindex(L, index);

    tracking.use(1);
    sol::optional<sol::table> try_table = sol::stack::check_get<sol::table>(L, absolute_index, handler);

    if(!try_table) return false;

    sol::table& table = *try_table;

    auto try_on_load = table["on_load"];
    if(!try_on_load.valid()) return false;
    sol::optional<sol::protected_function> try_on_load_fn = try_on_load;
    if(!try_on_load_fn) return false;

    return true;
}

Module sol_lua_get(
    sol::types<Module>, lua_State* L,
    int index, sol::stack::record& tracking
) {
    int absolute_index = lua_absindex(L, index);

    tracking.use(1);
    sol::table t = sol::stack::get<sol::table>(L, absolute_index);

    sol::protected_function on_load = t["on_load"];

    return Module {
        .on_load = on_load,
    };
}

Glue::Glue() : lua() {
    lua.open_libraries(sol::lib::base);
}

void Glue::discoverModule(std::string name, std::filesystem::path load_from) {
    sol::protected_function_result moduleResult = lua.do_file(load_from.string());
    if(moduleResult.valid()) {
        Module mod = moduleResult;
        ModuleEntry entry = { { mod }, load_from };
        modules[name] = entry;
    } else {
        sol::error err = moduleResult;
        printf("Lua error in %s: %s\n", load_from.string().c_str(), err.what());
    }
}

void Glue::loadModule(std::string name) {
    if(modules.contains(name)) {
        Module m = modules[name];
        m.on_load();
    } else {
        printf("Tried to load module %s, but no such module was discovered!\n", name.c_str());
    }
}
