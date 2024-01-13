UUIDgenerate <- function(use.time = NA, n = 1L, output = c("string", "raw", "uuid"))
    .Call(UUID_gen, n, switch(match.arg(output), string = 0L, raw = 1L, uuid = 2L),
          if (isTRUE(use.time)) 1L else if (isTRUE(!use.time)) 4L else NA_integer_, NULL)

UUIDfromName <- function(namespace, name, type = c("sha1", "md5"),
	     output = c("string", "raw", "uuid")) {
    ns <- as.UUID(namespace)
    if (length(ns) != 1 || any(is.na(ns)))
        stop("namespace must be a single, valid UUID")
    .Call(UUID_gen, name,
          switch(match.arg(output),
                 string = 0L, raw = 1L, uuid = 2L),
	  switch(match.arg(type), sha1 = 5L, md5 = 3L), ns)
}

UUIDparse <- function(what, output = c("uuid", "string", "raw", "logical"))
    .Call(UUID_parse, what,
          switch(match.arg(output),
                 string = 0L, raw = 1L, uuid = 2L, logical = 3L))

UUIDvalidate <- function(what) UUIDparse(what, output="logical")

UUID2string <- function(what) .Call(UUID_unparse, what, 0L)

as.character.UUID <- function(x, ...) .Call(UUID_unparse, x, 0L)

as.UUID <- function(x, ...) UseMethod("as.UUID")

as.UUID.UUID <- function(x, ...) x
as.UUID.raw <- function(x, ...) .Call(UUID_unparse, x, 1L)
as.UUID.default <- function(x, ...) UUIDparse(as.character(x), output="uuid")

as.raw.UUID <- function(x) .Call(UUID_unparse, x, 2L)

rep.UUID <- function(x, ...) {
    x <- rep(unclass(x), ...)
    class(x) <- "UUID"
    x
}

c.UUID <- function(...) {
    l <- lapply(list(...), function(x) if (inherits(x, "UUID")) unclass(x) else x)
    x <- do.call(base::c, l)
    if (is.complex(x)) class(x) <- "UUID"
    x
}

unique.UUID <- function (x, incomparables = FALSE, ...) {
    x <- unique(unclass(x), incomparables=incomparables, ...)
    class(x) <- "UUID"
    x
}

print.UUID <- function(x, ...) {
    if (length(x) == 1L)
        cat("UUID: ", as.character.UUID(x), "\n", sep='')
    else {
        cat("UUID vector:\n")
        print(as.character.UUID(x), ...)
    }
    invisible(x)
}

`[.UUID` <- function(x, i, ...) {
    x <- unclass(x)[i, ...]
    class(x) <- "UUID"
    x
}

`[<-.UUID` <- function(x, i, ..., value) {
    x <- unclass(x)
    x[i, ...] <- as.UUID(value)
    class(x) <- "UUID"
    x
}

`[[.UUID` <- function(x, i, ...) {
    x <- unclass(x)[[i, ...]]
    class(x) <- "UUID"
    x
}

`[[<-.UUID` <- function(x, i, ..., value) {
    x <- unclass(x)
    x[[i, ...]] <- as.UUID(value)
    class(x) <- "UUID"
    x
}

Ops.UUID <- function(e1, e2) stop(.Generic, " operator is not supported on UUIDs")
Math.UUID <- function(x, ...) stop(.Generic, " is not supported on UUIDs")
Complex.UUID <- function(z) stop(.Generic, " is not supported on UUIDs")
Summary.UUID <- function(..., na.rm = FALSE) stop(.Generic, " is not supported on UUIDs")

is.UUID <- function(x) inherits(x, "UUID")

`==.UUID` <- function(e1, e2) {
    if (inherits(e1, "UUID") && inherits(e2, "UUID"))
        .Call(UUID_cmp, e1, e2, 0L)
    else
        as.character(e1) == as.character(e2)
}

`!=.UUID` <- function(e1, e2) !`==.UUID`(e1, e2)

## We cannot use native is.na, because we only reserve (NA,NA), yet R will flag
## (x,NA) or (NA, x) in the complex case
is.na.UUID <- function(x) .Call(UUID_is_NA, x)
