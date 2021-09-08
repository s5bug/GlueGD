(* Geometry Dash Version -> Geometry Dash EXE Directory -> Config Directory -> Config *)
type config <- int -> string -> string -> { dll_path : string, module_search_path : list string }

type mod <- {
    on_enable : () -> ()
}
