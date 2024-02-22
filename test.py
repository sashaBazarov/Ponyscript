

def extract_additional_files(file_path):
        with open(file_path, 'r') as file:
            additional_files = []
            start_tag_found = False

            for line in file:
                if start_tag_found:
                    if line.strip() == '[Additional Files end]':
                        break
                    additional_files.append(line.strip())
                elif line.strip() == '[Additional Files start]':
                    start_tag_found = True

            return additional_files


print(extract_additional_files("C:\pony\.ponycfg"))