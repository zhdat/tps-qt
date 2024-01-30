#!/usr/bin/env bash
SCRIPT_NAME=$(basename $0)
SCRIPT_FULLPATH=$(realpath $0)
if [[ $SCRIPT_NAME != cppcheck.sh && $SCRIPT_NAME != guicheck.sh ]]; then
  echo "Error: cannot detect whether this is CPPCHECK or GUICHECK" > /dev/stderr
  echo "Note: program base name should be cppcheck.sh or guicheck.sh"  > /dev/stderr
  exit 1
fi
GUI=no
[[ $SCRIPT_NAME = "guicheck.sh" ]] && GUI=yes

#
# Parse command line, set options
#

function usage() {
  local label="C++"
  [[ $GUI = yes ]] && label="GUI"
  cat <<EOF
Usage: {cpp,gui}check.sh [OPTIONS] archive [archive ...]

 Check if an archive is ready for submission to teachers of $label labs @ Ensicaen

 Called as cppcheck.sh, will consider a C++ lab.

 Called as guicheck.sh, will consider a GUI lab.

OPTIONS
           -h : Print this help
        -w NN : Align output on NN columns
           -q : Quiet mode (no explanations)
           -f : Only print failed tests

AUTHORS
    Loic Simon (GREYC - Ensicaen)
    Sebastien Fourey (GREYC - Ensicaen)

EOF
  exit 0
}

ncols=$(tput cols)
display_explanations=yes
more_options=yes
failed_only=no
OPTIONS=""
while [[ $more_options = yes ]]; do
  case $1 in
    -w ) shift; ncols=$1; shift ; OPTIONS="${OPTIONS} -w $ncols" ;;
    -q ) shift; display_explanations=no ; OPTIONS="${OPTIONS} -q" ;;
    -h ) usage ;;
    -f ) shift; failed_only=yes ; OPTIONS="${OPTIONS} -f" ; ;;
    *) more_options=no
  esac
done

[[ $# = 0 || "$1" = -h || "$1" = --help ]] && usage

while [[ $# -gt 1 ]]; do
  $0 $OPTIONS "$1"
  shift
  echo
done

arxiv="$1"
fullname_arxiv=$(realpath "$arxiv")
echo Processing $(basename "$arxiv")
case ${arxiv} in
  *.tar ) extension=tar ;;
  *.tgz ) extension=tgz ;;
  *.zip ) extension=zip ;;
  * ) extension=invalid ;;
esac

declare -A list_commands=( [tar]="tar xf" [tgz]="tar xzf" [zip]="unzip" )
DIR=$(mktemp -d -t LAB.XXXX)
ISSUE=0

function dos2unix()
{
  local tmp=$(mktemp -t dos2unix-XXXXXXXXX.txt)
  mv "$1" $tmp
  cat $tmp | tr -d '\r' > "$1"
  rm -f $tmp
}

function print_fill() {
  local head=$1
  local tail=$2
  local width=$((ncols-${#head}-${#tail}))
  local s=$(printf "%-${width}s")
  echo -e ${head}"${s// /.}"${tail}
}

function print_center_fill() {
  local message=$1
  local ncolsleft=$(((ncols-6-${#message})/2))
  local ncolsright=$((ncols-6-${#message}-ncolsleft))
  local head=$(printf "%-${ncolsleft}s")
  local tail=$(printf "%-${ncolsright}s")
  echo -e "***${head}${message}${tail}***"
}

function print_warning() {
  print_center_fill "WARNING"
  print_center_fill ""
  print_center_fill "This archive IS NOT READY for submission."
}

function clean_tmpdir() {
  rm -r $DIR
}

function terminate_script() {
  if [[ $ISSUE = 1 ]]; then
    print_warning
  else
    echo "Archive is ready for submission."
  fi
  clean_tmpdir
  exit $ISSUE
}

function display_test_result() {
  local result="$1"
  local title="$2"
  local explanations="$3"
  local should_quit_on_failure="${4-false}"
  local message="Checking for ${title}"
  if [[ $result != 0 ]]; then
    ISSUE=1
    print_fill "${message}" [FAILED]
    if [[ -n "$explanations" && "$display_explanations" != no ]]; then
      echo -e $explanations
    fi
    if [[ $should_quit_on_failure = true ]]; then
      terminate_script
    fi
  else
    [[ $failed_only = no ]] && print_fill "${message}" [SUCCESS]
  fi
  return $result
}

function ignore_line() {
  local line="$1"
  [[ "$line" =~ ^ignore\  ]]
}

function check_arxiv_exists() {
  [[ -f $fullname_arxiv ]]; result=$?
  display_test_result $result "existing arxiv" "Your arxiv ${fullname_arxiv} does not exist" "true"
}

function check_extension() {
  [ -v list_commands[$extension] ]; result=$?
  local explanations="${extension} is not a valid extension (${!list_commands[@]})"
  display_test_result $result "extension" "$explanations" "true"
}


function uncompress() {
  cd $DIR
  local command=${list_commands[$extension]}
  ${command} "${fullname_arxiv}" &> /dev/null; result=$?
  local explanations="Archive cannot be extrated with command '${command}'"
  display_test_result $result "arxiv extraction" "$explanations" "true"
}


function check_root() {
  local command_subdirs="find . -mindepth 1 -maxdepth 1 -type d"
  local subdirs=$($command_subdirs)
  local nsubdirs=$($command_subdirs | wc -l)
  (( $nsubdirs == 1 )); root_exists=$?
  display_test_result $root_exists "arxiv root" "The root should be a single folder, not $nsubdirs (Found: $subdirs)"
  if [[ $root_exists = 0 ]]; then
    cd_to_root
    [[ $subdirs =~ ^(./)?[a-zA-Z-]*"_"?[a-zA-Z-]*$ ]]; result=$?
    display_test_result $result "arxiv root name" "The root filename should be your two names separated by a single '_' e.g \"FOUREY_SIMON\" and not $subdirs"
  fi
}

function cd_to_root() {
  cd */
}

function is_object() {
  file "$1" | grep -Eq "(relocatable)|(executable)|(shared)"
}

function check_unwanted_files() {
  local unexpected_files=$(mktemp -t cppcheck_unexpected_files.XXXXXX)
  rm -f ${unexpected_files}
  ls -A | grep "^\." >> ${unexpected_files}
  find . -name "*.o" -o -name "*~" >> ${unexpected_files}
  while read filename; do
    if is_object "$filename"; then
      echo "$filename" >> ${unexpected_files}
    fi
  done < <(find . -type f)

  local explanations=""
  local result=0
  [[ -s ${unexpected_files} ]] && result=1
  if [[ $result != 0 ]]; then
    explanations="The following files/directories should not be present:\n"
    while read line; do
      explanations="${explanations}${line}\n"
    done < ${unexpected_files}
  fi
  rm -f ${unexpected_files}
  display_test_result $result "unwanted files" "$explanations"
}

function check_arxiv() {
  check_arxiv_exists
  check_extension
  uncompress
  check_root
  [[ $GUI = no ]] && check_unwanted_files
}

function check_file_list_present() {
  if [[ -f files.txt ]]; then
    local result=0
    echo >> files.txt
  else
    local result=1
  fi
  display_test_result $result files.txt "files.txt was not found" "true"
}

function check_files_in_list() {
  local result=0
  local explanations="the following files are listed in 'files.txt' but do not exist:"
  while read file; do
    ignore_line "$file" && continue
    if [[  -n "$file" && (! -f "$file") ]]; then
      result=1
      explanations="${explanations}\n ${file}"
    fi
  done < files.txt
  display_test_result $result "listed files present" "$explanations"
}

function check_valid_files_listed() {
  local result=0
  local explanations="the following files are not suitable in files.txt:"
  local file=""
  local base=""
  while read file; do
    ignore_line "$file" && continue
    if is_object "$file" ; then
      result=1
      explanations="${explanations}\n ${file}"
      continue
    fi
    base=$(basename "$file")
    case "$base" in
      files.txt | *~ | *.o | moc_* | *.ui  | *.user | *.ts | *.qm )
	result=1
	explanations="${explanations}\n ${file}"
	;;
    esac
  done < files.txt
  display_test_result $result "only valid files listed" "$explanations"
}

function check_nosource_outside_list() {
  local result=0
  local explanations="the following files are not listed in 'files.txt' but do exist: "
  local file
  while read file; do
    [[ "$file" =~ files.txt$ ]] && continue
    file=${file#./}
    if ! grep -qE "^(ignore[ ]+)?(./)?${file}\$" files.txt ; then
      result=1
      explanations="$explanations $file"
    fi
  done < <(find . -type f -not -path '*/\.*' )
  display_test_result $result "unlisted files" "$explanations"
}

function check_makefile() {
  local explanations=""
  local result=1
  if grep -qi makefile files.txt ; then
    result=0
  else
    explanations="No Makefile listed in files.txt"
  fi
  display_test_result $result "makefile listed" "$explanations"
}

function check_qt_project() {
  local explanations=""
  local result=1
  local makefile=no
  grep -qi makefile files.txt && makefile=yes
  local qt_project=no
  egrep -qE '.+\.pro$' files.txt && qt_project=yes
  local qt_cmake=no
  egrep -qE 'CMakeLists.txt$' files.txt && qt_cmake=yes

  if [[ $qt_project = yes || $qt_cmake = yes ]]; then
    result=0
  else
    explanations="No Qt project file (.pro or CMakeLists.txt) listed in files.txt"
  fi
  if [[ $makefile = yes ]]; then
    result=1
    [[ -n $explanations ]] && explanations+="\n"
    explanations="${explanation}A Makefile is listed in files.txt"
  fi
  display_test_result $result "qt project or CMakeLists.txt file listed" "$explanations"
}

function check_files_h_cpp_order() {
  # Check that no .c ou .cpp comes before a .h
  local result=0
  local explanations=""
  local c_cpp_names=()
  local filename=""
  local base=""
  local found=""
  while read file; do
    ignore_line "$file" && continue
    [[ -d "$file" ]] && continue
    if [[ "$file" =~ \.c$ ]]; then
      filename=$(basename "$file")
      c_cpp_names+=($filename)
    elif [[ "$file" =~ \.cpp$ ]]; then
      filename=$(basename "$file")
      c_cpp_names+=($filename)
    elif [[ "$file" =~ \.h$ ]]; then
      base=$(basename "$file" .h)
      found=""
      for previous in ${c_cpp_names[@]} ; do
	[[ "$previous" = "${base}.c" ]] && found="${base}.c"
	[[ "$previous" = "${base}.cpp" ]] && found="${base}.cpp"
      done
      if [[ -n "$found" ]]; then
	result=1
	[[ -n "$explanations" ]] && explanations="${explanations}\n"
	explanations="${explanations}${base}.h must be placed ahead of ${found} in files.txt"
      fi
    fi
  done < files.txt
  display_test_result $result "listed files order" "$explanations"
}

function check_file_list() {
  check_file_list_present && dos2unix files.txt
  check_files_in_list
  check_files_h_cpp_order
  [[ $GUI = yes ]] && check_valid_files_listed
  [[ $GUI = yes ]] && check_qt_project
  [[ $GUI = no ]] && check_makefile
  [[ $GUI = no ]] && check_nosource_outside_list
}

check_arxiv
check_file_list
terminate_script
