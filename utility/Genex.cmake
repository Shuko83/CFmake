function(cstoolkit_genex_list_intersection lista listb outputlist)
    set(aminusb "$<LIST:REMOVE_ITEM,${lista},${listb}>")
    
    set(bminusa "$<LIST:REMOVE_ITEM,${listb},${lista}>")

    set(${outputlist} "${lista};${listb}")
    set(${outputlist} "$<LIST:REMOVE_DUPLICATES,${${outputlist}}>")
    set(${outputlist} "$<LIST:REMOVE_ITEM,${${outputlist}},${aminusb};${bminusa}>")
    set(${outputlist} "$<JOIN:${${outputlist}},;>") # remove empty elements

    set(${outputlist} "${${outputlist}}" PARENT_SCOPE)
endfunction()
