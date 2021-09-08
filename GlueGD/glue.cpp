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

    auto try_on_enable = table["on_enable"];
    if(!try_on_enable.valid()) return false;
    sol::optional<sol::protected_function> try_on_enable_fn = try_on_enable;
    if(!try_on_enable_fn) return false;

    return true;
}

Module sol_lua_get(
    sol::types<Module>, lua_State* L,
    int index, sol::stack::record& tracking
) {
    int absolute_index = lua_absindex(L, index);

    tracking.use(1);
    sol::table t = sol::stack::get<sol::table>(L, absolute_index);

    sol::protected_function on_enable = t["on_enable"];

    return Module {
        .on_enable = on_enable,
    };
}

Glue::Glue() : lua() {
    lua.open_libraries(sol::lib::base);
}

void Glue::load_module(std::string name, std::filesystem::path load_from) {
    sol::protected_function_result module_result = lua.do_file(load_from.string());
    if(module_result.valid()) {
        Module mod = module_result;
        ModuleEntry entry = { { mod }, load_from };
        m_modules[name] = entry;
    } else {
        sol::error err = module_result;
        printf("Lua error in %s: %s\n", load_from.string().c_str(), err.what());
    }
}

void Glue::enable_module(std::string name) {
    if(m_modules.contains(name)) {
        Module m = m_modules[name];
        m.on_enable();
    } else {
        printf("Tried to load module %s, but no such module was discovered!\n", name.c_str());
    }
}

const std::map<std::string, ModuleEntry>& Glue::modules() const {
    return m_modules;
}
