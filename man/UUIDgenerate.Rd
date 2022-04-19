\name{UUIDgenerate}
\alias{UUIDgenerate}
\alias{UUIDparse}
\alias{UUIDvalidate}
\title{
  UUID Functions
}
\description{
  \code{UUIDgenerate} generates new Universally Unique Identifiers. It
  can be either time-based or random.

  \code{UUIDparse} parses one of more UUIDs it string form and converts
  them to other internal formats.

  \code{UUIDvalidate} checks the valitiy of UUIDs in string form.
}
\usage{
UUIDgenerate(use.time = NA, n = 1L, output = c("string", "raw", "uuid"))
UUIDparse(what, output = c("uuid", "string", "raw", "logical"))
UUIDvalidate(what)
}
\arguments{
  \item{use.time}{logical, if \code{TRUE} then time-based UUID is
    generated, if \code{FALSE} then a random UUID is generated, if
    \code{NA} then random one is generated if a sufficiently reliable
    source of random numbers can be found, otherwise a time-based UUID
    is generated.}
  \item{n}{integer, number of UUIDs to generate.}
  \item{output}{type of the output. Valid types are: \code{"string"} for
    a character vector with UUIDs in textual representation (always
    lowercase), \code{"raw"} for a vector or matrix of raw bytes,
    \code{"uuid"} for an object of the class \code{\link{UUID}} and
    \code{"logical"} which only reports failure/success of the parsing,
    but not the actual values.}
  \item{what}{character vector which will be parsed into UUIDs.}    
}
\value{
  \code{UUIDgenerate} and \code{UUIDparse} values depend on the
  \code{output} argument as follows:
  
  \item{\code{"string"}}{character vector with each element UUID in
    lowercase form, for \code{UUIDparse} strings that cannot be parsed
    will result in \code{NA} values}
  \item{\code{"raw"}}{raw vector with the UUIDs stores each as 16 bytes
    seqeuntially. If the output is more than one UUID then the result is a
    raw matrix with 16 rows and as many columns as there are input
    elements.}
  \item{\code{"uuid"}}{object of the class \code{\link{UUID}} which is a
    vector of UUIDs in 128-bit internal representation.}
  \item{\code{"logical"}}{only supported in \code{UUIDparse} and return
    code \code{TRUE} for valid UUID, \code{FALSE} for invalid input and
    \code{NA} for \code{NA} input.}

  \code{UUIDvalidate} is just a shorthand for
  \code{UUIDparse(what, output="logical")}.
}
\author{
  Simon Urbanek, based on libuuid by Theodore Ts'o.
}
\note{
  The first argument is not \code{n} for historical reasons, beause the
  first version did only generate a single UUID.
}
\examples{
UUIDgenerate()
UUIDgenerate(TRUE)
UUIDgenerate(FALSE)

## see if the randomness is any good
length(unique(UUIDgenerate(n=1000)))

## generate a native UUID vector
(u <- UUIDgenerate(n=3, output="uuid"))

as.character(u)
as.raw(u[1])

UUIDgenerate(output="raw")

## see ?UUID for more examples on UUID objects
}
\keyword{manip}
