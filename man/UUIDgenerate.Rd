\name{UUIDgenerate}
\alias{UUIDgenerate}
\title{
  Generate UUID
}
\description{
  \code{UUIDgenerate} generates a new Universally Unique Identifier. It
  can be either time-based or random.
}
\usage{
UUIDgenerate(use.time = NA, n = 1L)
}
\arguments{
  \item{use.time}{logical, if \code{TRUE} then time-based UUID is
    generated, if \code{FALSE} then a random UUID is generated, if
    \code{NA} then random one is generated if a sufficiently reliable
    source of random numbers can be found, otherwise a time-based UUID
    is generated.}
  \item{n}{integer, number of UUIDs to generate. Must be in [0, 2^31)}
}
%\details{
%}
\value{
  String, UUID in lowercase form.
}
%\references{
%}
\author{
  Simon Urbanek, based on libuuid by Theodore Ts'o.
}
%\note{
%}
\examples{
UUIDgenerate()
UUIDgenerate(TRUE)
UUIDgenerate(FALSE)
## see if the randomness is any good
length(unique(UUIDgenerate(n=1000)))
}
\keyword{manip}
