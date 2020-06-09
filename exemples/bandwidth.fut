--
-- Project : Futhark host alloc
-- Author : El Kharroubi Michaël
-- Date : 2020  
--
let identity (input: i32) : i32 = input
-- Does nothing, it's only useful to avoid optimization.
entry main [n] (tab: [n]i32) : [n]i32 = map identity tab