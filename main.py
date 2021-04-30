import yaml
import os

config_path = './tex/config.yml'
head_path = './tex/head'
tail_path = './tex/tail'


def get_head():
    res = ""
    with open(head_path, 'r', encoding='utf-8') as f:
        lines = f.readlines()
        for line in lines:
            res += line
    return res


def get_tail():
    res = ""
    with open(tail_path, 'r', encoding='utf-8') as f:
        lines = f.readlines()
        for line in lines:
            res += line
    return res


def get_code(name, path):
    print(f'    - add code {name}: {path}')
    suf = path.split('.')[1]
    path = path
    res = '\inputminted[mathescape,linenos,numbersep=5pt,frame=lines,framesep=2mm]' + \
        '{' + suf + '}' + '{' + path + '}' + '\n'
    return res


def get_subsection(subsection):
    print(f'  - add subsection{subsection}')
    res = ""

    for name in subsection:
        res = res + '\\subsection' + '{' + name + '}' + '\n'
        path = subsection[name]
        res = res + get_code(name, path)
    return res


def get_section(content, section):
    print(f"- add section {section}")
    res = '\\section' + '{' + section + '}' + '\n'
    for subsection in content[section]:
        res = res + get_subsection(subsection)
    return res


def generate_config():
    sections = os.listdir('src')
    print(sections)

    content = {}
    for section in sections:
        subsections = os.listdir('src/' + section)
        print('  ', subsections)

        a = []
        for f in subsections:
            name = f.split('.')[0]
            path = 'src/' + section + '/' + f
            print('      ', name, path)

            a.append({name: path})
        content[section] = a
    config = {'content': content}

    with open('tex/config.yml', 'w') as f:
        yaml.safe_dump(config, f)

    return config


if __name__ == '__main__':
    config = generate_config()

    print(config)

    tex = ""

    tex = tex + get_head()

    content = config['content']
    for section in content:
        tex = tex + get_section(content, section)

    tex = tex + get_tail()

    with open('template.tex', 'w', encoding='utf-8') as f:
        f.write(tex)
